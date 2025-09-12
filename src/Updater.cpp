#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class Updater : public QObject {
    Q_OBJECT
public:
    Updater(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
    }

    void checkForUpdates() {
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

signals:
    void updateAvailable(QString newVersion);

private:
    QNetworkAccessManager *manager;
    QString currentVersion = "1.0.0"; // read from manifest
};
