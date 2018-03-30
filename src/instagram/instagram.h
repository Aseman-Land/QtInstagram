#ifndef INSTAGRAM_H
#define INSTAGRAM_H

#include <QObject>
#include <QNetworkAccessManager>

#include "libqinstagram_global.h"

#include <functional>

class LIBQINSTAGRAMSHARED_EXPORT Instagram : public QObject
{
    Q_OBJECT
    class Private;
    friend class InstagramQuery;

public:
    Instagram(const QString &clientId, QObject *parent = Q_NULLPTR);
    ~Instagram();

    template<typename T>
    using Callback = std::function<void (T)>;

    QUrl accessTokenUrl(const QString &redirect_uri);
    void setAccessToken(const QString &accessToken);

    void mediaReadComments(const QString &mediaId, Callback<QVariant> callback = Q_NULLPTR);
    void mediaGetMedia(const QString &mediaId, Callback<QVariant> callback = Q_NULLPTR);

    void userReadMedias(const QString &userId = QStringLiteral("self"), qint32 count = 20, qint64 minId = 0, qint64 maxId = 0, Callback<QVariant> callback = Q_NULLPTR);
    void userGetInfo(const QString &userId = QStringLiteral("self"), Callback<QVariant> callback = Q_NULLPTR);

    void search(const QString &query, qint32 count = 20, Callback<QVariant> callback = Q_NULLPTR);

protected:
    QNetworkAccessManager *networkAccessManager() const;

private:
    Private *p;
};

#endif // INSTAGRAM_H
