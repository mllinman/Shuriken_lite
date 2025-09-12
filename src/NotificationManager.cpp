#include "NotificationManager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

void NotificationManager::listNotifications() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://localhost:4242/notifications"));
    // Replace with your actual token source, e.g., member variable or function
    QString userToken = this->userToken; // Assuming 'userToken' is a member of NotificationManager
    req.setRawHeader("Authorization", "Bearer " + userToken.toUtf8());

    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QStringList list;
        if (!reply->error()) {
            QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
            for (auto v : arr) {
                QJsonObject obj = v.toObject();
                QString entry = QString("%1 %2").arg(obj["readFlag"].toInt() ? "✅" : "🔔").arg(obj["message"].toString());
                list << entry;
            }
        }
        emit notificationsListed(list);
        reply->deleteLater();
    });
}
