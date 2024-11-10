#include "alicloudapi.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

AliCloudAPI::AliCloudAPI(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this))
{
    // 设置你的API密钥
    accessKeyId = "your-access-key-id";
    accessKeySecret = "your-access-key-secret";

    connect(networkManager, &QNetworkAccessManager::finished, this, &AliCloudAPI::handleNetworkReply);
}

void AliCloudAPI::callAPI(const QString &prompt)
{
    // 阿里云API的URL
    QUrl url("https://your-api-endpoint.aliyuncs.com/api/v1/dialogue");

    // 请求体
    QJsonObject requestBody;
    requestBody["prompt"] = prompt;
    requestBody["max_tokens"] = 50;

    QJsonDocument jsonDoc(requestBody);
    QByteArray postData = jsonDoc.toJson();

    // 创建请求
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + accessKeyId).toUtf8());

    // 发送POST请求
    networkManager->post(request, postData);
}

void AliCloudAPI::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error()) {
        emit errorOccurred(reply->errorString());
    } else {
        // 处理响应
        QByteArray data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
        QJsonObject jsonObject = jsonResponse.object();
        QString responseText = jsonObject.value("response").toString();
        emit responseReceived(responseText);
    }

    reply->deleteLater();
}
