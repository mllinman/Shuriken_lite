#include <QApplication>
#include <QFile>
#include "src/MainWindow.h"

// ...existing code...
int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QApplication::setApplicationName("Shuriken_lite");
    QApplication::setOrganizationName("BulletDrop_Studio");
    QApplication::setApplicationVersion("1.0.0");
    // Load dark theme
    QFile qss(":/resources/darktheme.qss");
    if (qss.open(QFile::ReadOnly)) {
        app.setStyleSheet(qss.readAll());
    }

    MainWindow w;
    w.setWindowTitle("Shuriken — C++ Builder");
    w.resize(1000, 700);
    w.show();

    return app.exec();
}

// If you need the installer-related code, move it into a function like below:

/*
bool createInstaller(const QString& appName, const QString& destExe, const QString& installDir,
                     const QString& uninstallerPath, const QString& publisher, const QString& version,
                     const QString& description, QString& log) {
    // ...installer code here...
    return true;
}
*/

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

