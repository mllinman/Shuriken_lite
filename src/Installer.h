#pragma once
#include <QString>

class Installer {
public:
    Installer(QString appName, QString exePath);
    bool createInstaller(QString outPath, QString &log);

private:
    QString appName;
    QString exePath;
};
