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
    query.addQueryItem("client_id", p->clientId);
    query.addQueryItem("redirect_uri", redirect_uri);
    query.addQueryItem("response_type", "token");

    QUrl url("https://www.instagram.com/oauth/authorize/");
    url.setQuery(query);

    return url;
}

void Instagram::setAccessToken(const QString &accessToken)
{
    p->accessToken = accessToken;
}

void Instagram::mediaReadComments(const QString &mediaId, Callback<QVariant> callback)
{
    InstagramQuery *query = new InstagramQuery("/media/:media-id/comments", this);
    query->bindValue(":media-id", mediaId);
    query->addValue("access_token", p->accessToken);
    query->connect(query, &InstagramQuery::finished, this, callback);
    query->exec();
}

void Instagram::getMediaId(const QUrl &url, Callback<QString> callback)
{
    InstagramQuery *query = new InstagramQuery(QString(), this);
    query->getMediaId("https://api.instagram.com/oembed/?url=" + url.toString(), callback);
}

QNetworkAccessManager *Instagram::networkAccessManager() const
{
    return p->manager;
}

Instagram::~Instagram()
{
    delete p;
}
