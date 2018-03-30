#include "instagram.h"
#include "instagramquery.h"

#include <QUrlQuery>
#include <QDebug>

class Instagram::Private
{
public:
    QString clientId;
    QString accessToken;
    QNetworkAccessManager *manager;
};

Instagram::Instagram(const QString &clientId, QObject *parent) :
    QObject(parent)
{
    p = new Private;
    p->clientId = clientId;
    p->manager = new QNetworkAccessManager(this);
}

QUrl Instagram::accessTokenUrl(const QString &redirect_uri)
{
    QUrlQuery query;
    query.addQueryItem( QStringLiteral("client_id"), p->clientId);
    query.addQueryItem( QStringLiteral("redirect_uri"), redirect_uri);
    query.addQueryItem( QStringLiteral("response_type"), QStringLiteral("token"));

    QUrl url( QStringLiteral("https://www.instagram.com/oauth/authorize/"));
    url.setQuery(query);

    return url;
}

void Instagram::setAccessToken(const QString &accessToken)
{
    p->accessToken = accessToken;
}

void Instagram::mediaReadComments(const QString &mediaId, Callback<QVariant> callback)
{
    InstagramQuery *query = new InstagramQuery( QStringLiteral("/media/:media-id/comments"), this);
    query->bindValue( QStringLiteral(":media-id"), mediaId);
    query->addValue( QStringLiteral("access_token"), p->accessToken);
    if(callback) query->connect(query, &InstagramQuery::finished, this, callback);
    query->exec();
}

void Instagram::mediaGetMedia(const QString &mediaId, Callback<QVariant> callback)
{
    InstagramQuery *query = new InstagramQuery( QStringLiteral("/media/:media-id"), this);
    query->bindValue( QStringLiteral(":media-id"), mediaId);
    query->addValue( QStringLiteral("access_token"), p->accessToken);
    if(callback) query->connect(query, &InstagramQuery::finished, this, callback);
    query->exec();
}

void Instagram::userReadMedias(const QString &userId, qint32 count, qint64 minId, qint64 maxId, Callback<QVariant> callback)
{
    InstagramQuery *query = new InstagramQuery( QStringLiteral("/users/:user-id/media/recent"), this);
    query->bindValue( QStringLiteral(":user-id"), userId);
    if(count) query->addValue( QStringLiteral("count"), QString::number(count) );
    if(minId) query->addValue( QStringLiteral("min_id"), QString::number(minId) );
    if(maxId) query->addValue( QStringLiteral("max_id"), QString::number(maxId) );
    query->addValue( QStringLiteral("access_token"), p->accessToken);
    if(callback) query->connect(query, &InstagramQuery::finished, this, callback);
    query->exec();
}

void Instagram::userGetInfo(const QString &userId, Callback<QVariant> callback)
{
    InstagramQuery *query = new InstagramQuery( QStringLiteral("/users/:user-id"), this);
    query->bindValue( QStringLiteral(":user-id"), userId);
    query->addValue( QStringLiteral("access_token"), p->accessToken);
    if(callback) query->connect(query, &InstagramQuery::finished, this, callback);
    query->exec();
}

void Instagram::search(const QString &q, qint32 count, Callback<QVariant> callback)
{
    InstagramQuery *query = new InstagramQuery( QStringLiteral("/users/search"), this);
    query->addValue( QStringLiteral("access_token"), p->accessToken);
    query->addValue( QStringLiteral("q"), q );
    if(count) query->addValue( QStringLiteral("count"), QString::number(count) );
    if(callback) query->connect(query, &InstagramQuery::finished, this, callback);
    query->exec();
}

QNetworkAccessManager *Instagram::networkAccessManager() const
{
    return p->manager;
}

Instagram::~Instagram()
{
    delete p;
}
