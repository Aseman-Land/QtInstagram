#include "instagramquery.h"
#include "instagram.h"

#include <QHash>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QPointer>
#include <QJsonDocument>

class InstagramQuery::Private
{
public:
    QList< QPair<QString,QString> > values;
    QHash<QString, QString> boundedValues;
    QString method;

    QPointer<Instagram> instagram;
};

InstagramQuery::InstagramQuery(const QString &method, Instagram *parent) :
    QObject(parent)
{
    p = new Private;
    p->instagram = parent;
    p->method = method;
}

void InstagramQuery::addValue(const QString &key, const QString &value)
{
    QPair<QString, QString> pair;
    pair.first = key;
    pair.second = value;

    p->values << pair;
}

void InstagramQuery::removeValue(const QString &key)
{
    for(int i=0; i<p->values.count(); i++)
    {
        const QPair<QString, QString> pair = p->values.at(i);
        if(pair.first != key)
            continue;

        p->values.removeAt(i);
        i--;
    }
}

QString InstagramQuery::value(const QString &key)
{
    for(const QPair<QString, QString> &pair: p->values)
        if(pair.first == key)
            return pair.second;
    return QString();
}

void InstagramQuery::bindValue(const QString &key, const QString &value)
{
    p->boundedValues[key] = value;
}

void InstagramQuery::unbindValue(const QString &key)
{
    p->boundedValues.remove(key);
}

QString InstagramQuery::boundedValue(const QString &key)
{
    return p->boundedValues.value(key);
}

void InstagramQuery::exec()
{
    QString method = p->method;
    QHashIterator<QString, QString> bi(p->boundedValues);
    while(bi.hasNext())
    {
        bi.next();
        method.replace(bi.key(), bi.value());
    }

    QUrlQuery query;
    for(const QPair<QString, QString> &pair: p->values)
        query.addQueryItem(pair.first, pair.second);

    QUrl url("https://api.instagram.com/v1" + method);
    url.setQuery(query);

    sendRequest(url, [this](const QByteArray &data){
        Q_EMIT finished( QJsonDocument::fromJson(data).toVariant() );
    });
}

void InstagramQuery::getMediaId(const QUrl &url, std::function<void (const QString &result)> callback)
{
    sendRequest(url, [this, callback](const QByteArray &data){
        QVariantMap res = QJsonDocument::fromJson(data).toVariant().toMap();
        const QString mediaId = res.value("media_id").toString();
        callback( mediaId.left(mediaId.indexOf("_")) );
    });
}

void InstagramQuery::sendRequest(const QUrl &url, std::function<void (const QByteArray &)> callback)
{
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply *reply = p->instagram->networkAccessManager()->get(request);
    connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &InstagramQuery::error);
    connect(reply, &QNetworkReply::sslErrors, this, &InstagramQuery::sslErrors);
    connect(reply, &QNetworkReply::readyRead, this, [this, reply, callback](){
        reply->deleteLater();
        deleteLater();
        callback(reply->readAll());
    });
}

InstagramQuery::~InstagramQuery()
{
    delete p;
}
