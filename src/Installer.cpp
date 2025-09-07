#include "Installer.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QStandardPaths>
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <comdef.h>
#include <objbase.h>


Installer::Installer(QString appName, QString exePath, QString manifestPath)
    : appName(appName), exePath(exePath), manifestPath(manifestPath) {}

bool Installer::createInstaller(QString outPath, QString &log) {
    QFile exeFile(exePath);
    if (!exeFile.exists()) {
        log.append("Target executable not found.\n");
        return false;
    }

    // Declare publisher, version, and description
    QString publisher = "Shuriken Builder";
    QString version   = "1.0.0";
    QString description = "Application built with Shuriken";

    if (!manifestPath.isEmpty() && QFile::exists(manifestPath)) {
        QFile manifestFile(manifestPath);
        if (manifestFile.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(manifestFile.readAll());
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj.contains("publisher")) publisher = obj["publisher"].toString();
                if (obj.contains("version"))   version   = obj["version"].toString();
                if (obj.contains("description")) description = obj["description"].toString();
            }
        }
    } else {
        log.append("No manifest file found, using defaults.\n");
    }

    // 🔹 Define install paths
    QString programFiles = QString::fromWCharArray(_wgetenv(L"ProgramFiles"));
    QString installDir = programFiles + "\\" + appName;

    QDir().mkpath(installDir);

    // 🔹 Copy main executable
    QString destExe = installDir + "\\" + QFileInfo(exePath).fileName();
    if (!QFile::copy(exePath, destExe)) {
        log.append("Failed to copy executable to install dir.\n");
        return false;
    }
    log.append("Installed " + appName + " to: " + installDir + "\n");

    // 🔹 Copy generic uninstaller
    QString uninstallerPath = installDir + "\\Uninstaller.exe";
    if (!QFile::copy("resources/Uninstaller.exe", uninstallerPath)) {
        log.append("Failed to copy uninstaller.\n");
    } else {
        log.append("Uninstaller copied.\n");
    }

    // 🔹 Registry metadata
    if (!registerUninstaller(appName, installDir, uninstallerPath, destExe, publisher, version, description))
        log.append("Failed to register uninstaller.\n");
    else
        log.append("Uninstaller registered in registry with versioning.\n");

    return true;
}

bool Installer::registerUninstaller(QString appName, QString installDir,
                                    QString uninstallerPath, QString installedExe,
                                    QString publisher, QString version, QString description) {
    HKEY hKey;
    QString uninstallKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName;

    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, reinterpret_cast<LPCWSTR>(uninstallKey.utf16()),
                        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        return false;
    }

    QString displayName = appName;
    QString uninstallCommand = "\"" + uninstallerPath + "\"";

    // Calculate estimated size
    QFileInfo exeInfo(installedExe);
    DWORD estimatedSizeKB = exeInfo.size() / 1024;
    QString installDate = QDate::currentDate().toString("yyyyMMdd");

    // Write registry values
    RegSetValueExW(hKey, L"DisplayName", 0, REG_SZ,
                   (BYTE*)displayName.utf16(), (displayName.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"Publisher", 0, REG_SZ,
                   (BYTE*)publisher.utf16(), (publisher.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"DisplayVersion", 0, REG_SZ,
                   (BYTE*)version.utf16(), (version.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"InstallLocation", 0, REG_SZ,
                   (BYTE*)installDir.utf16(), (installDir.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"UninstallString", 0, REG_SZ,
                   (BYTE*)uninstallCommand.utf16(), (uninstallCommand.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"InstallDate", 0, REG_SZ,
                   (BYTE*)installDate.utf16(), (installDate.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"EstimatedSize", 0, REG_DWORD,
                   (BYTE*)&estimatedSizeKB, sizeof(DWORD));
    RegSetValueExW(hKey, L"Comments", 0, REG_SZ,
                   (BYTE*)description.utf16(), (description.size() + 1) * sizeof(wchar_t));

    RegCloseKey(hKey);
    return true;
}


