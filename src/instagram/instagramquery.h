#ifndef INSTAGRAMQUERY_H
#define INSTAGRAMQUERY_H

#include <QObject>
#include <QNetworkReply>

#include <functional>

class InstagramQuery : public QObject
{
    Q_OBJECT
    class Private;

public:
    InstagramQuery(const QString &method, class Instagram *parent = Q_NULLPTR);
    ~InstagramQuery();

    void addValue(const QString &key, const QString &value);
    void removeValue(const QString &key);
    QString value(const QString &key);

    void bindValue(const QString &key, const QString &value);
    void unbindValue(const QString &key);
    QString boundedValue(const QString &key);

    void getMediaId(const QUrl &url, std::function<void (const QString &result)> callback);

public Q_SLOTS:
    void exec();

Q_SIGNALS:
    void error(QNetworkReply::NetworkError);
    void sslErrors(const QList<QSslError> &errors);
    void finished(const QVariant &result);

protected:
    void sendRequest(const QUrl &url, std::function<void (const QByteArray &result)> callback);

private:
    Private *p;
};

#endif // INSTAGRAMQUERY_H
