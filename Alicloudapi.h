#ifndef ALICLOUDAPI_H
#define ALICLOUDAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

class AliCloudAPI : public QObject
{
    Q_OBJECT

public:
    explicit AliCloudAPI(QObject *parent = nullptr);
    void callAPI(const QString &prompt);

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString accessKeyId;
    QString accessKeySecret;
};

#endif // ALICLOUDAPI_H
