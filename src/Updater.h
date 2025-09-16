#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QString>

class Updater : public QObject {
    Q_OBJECT
public:
    Updater(QObject *parent = nullptr);
    void checkForUpdates();

signals:
    void updateAvailable(QString version);

private:
    QNetworkAccessManager *manager;
    QString currentVersion;
};

#endif // UPDATER_H