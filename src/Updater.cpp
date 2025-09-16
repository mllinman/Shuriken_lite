#include "Updater.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QNetworkRequest>

Updater::Updater(QObject *parent) : QObject(parent), currentVersion("1.0.0") {
    manager = new QNetworkAccessManager(this);
}

void Updater::checkForUpdates() {
    QUrl url("https://raw.githubusercontent.com/youruser/Shuriken/main/version.json");
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QString latestVersion = doc["version"].toString();
        if (latestVersion != currentVersion) {
            emit updateAvailable(latestVersion);
        }
        reply->deleteLater();
    });
}
