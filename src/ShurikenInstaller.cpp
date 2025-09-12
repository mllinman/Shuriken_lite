#include <QCoreApplication>
#include "Installer.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QString log;
    Installer installer("Shuriken", "bin/Shuriken.exe", "manifest_example.json");

    if (installer.createInstaller("ShurikenSetup.exe", log)) {
        qDebug() << "Shuriken installer created successfully.";
    } else {
        qDebug() << "Installer creation failed.";
    }

    return 0;
}
