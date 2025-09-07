#pragma once
#include <QString>

class Installer {
public:
    Installer(QString appName, QString exePath, QString manifestPath = "");
    bool createInstaller(QString outPath, QString &log);
    bool registerUninstaller(QString appName, QString installDir,
                            QString uninstallerPath, QString installedExe,
                            QString publisher, QString version, QString description);

private:
    QString appName;
    QString exePath;
    QString manifestPath;
};
