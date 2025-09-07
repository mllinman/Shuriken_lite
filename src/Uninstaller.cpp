#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QMessageBox>
#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
#include <QProcess>
#include <comdef.h>
#include <objbase.h>
#include "Installer.h"
#include "Uninstaller.h"

bool createShortcut(QString location, QString name, QString targetPath) {
    QString folder;
    if (location == "Desktop")
        folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    else if (location == "StartMenu")
        folder = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    else
        return false;

    QString lnkPath = folder + "\\" + name + ".lnk";
    CoInitialize(NULL);

    HRESULT hres;
    IShellLinkW* psl;

    // Initialize COM library
    hres = CoInitialize(NULL);
    if (FAILED(hres)) return false;

    // Create ShellLink object
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl);
    if (FAILED(hres)) {
        CoUninitialize();
        return false;
    }

    // Set the path to the target
    psl->SetPath(reinterpret_cast<LPCWSTR>(targetPath.utf16()));
    psl->SetDescription(L"Shortcut for " + reinterpret_cast<LPCWSTR>(name.utf16()));

    // Query for the IPersistFile interface
    IPersistFile* ppf;
    hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
    if (FAILED(hres)) {
        psl->Release();
        CoUninitialize();
        return false;
    }

    // Save the link
    hres = ppf->Save(reinterpret_cast<LPCWSTR>(lnkPath.utf16()), TRUE);
    ppf->Release();
    psl->Release();
    CoUninitialize();

    return SUCCEEDED(hres);
}
void deleteShortcut(QString location, QString name) {
    QString folder;
    if (location == "Desktop")
        folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    else if (location == "StartMenu")
        folder = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    else
        return;

    QString lnkPath = folder + "\\" + name + ".lnk";
    QFile::remove(lnkPath);
}

bool removeRegistryEntry(QString appName) {
    QString uninstallKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName;

    if (RegDeleteTreeW(HKEY_LOCAL_MACHINE, reinterpret_cast<LPCWSTR>(uninstallKey.utf16())) != ERROR_SUCCESS) {
        return false;
    }
    return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    QString appName = "MyApp"; // TODO: Replace dynamically or pass via args
    QString destExe = QCoreApplication::applicationFilePath();
    QStringList args = QCoreApplication::arguments();
    if (args.contains("--uninstall")) {
        // Run uninstallation
    } else {
        MessageBoxW(NULL, L"This is the uninstaller. To uninstall, run with --uninstall argument.", L"Uninstaller", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    // Find Program Files install dir
    QString programFiles = QString::fromWCharArray(_wgetenv(L"ProgramFiles"));
    QString installDir = programFiles + "\\" + appName;
    if (!QDir(installDir).exists()) {
        MessageBoxW(NULL, L"Installation directory not found.", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    // Delete shortcuts
    // 🔹 Helper: remove shortcuts
    void deleteShortcut(QString location, QString name) {
        QString folder;
        if (location == "Desktop")
            folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        else if (location == "StartMenu")
        folder = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);

    QString lnkPath = folder + "\\" + name + ".lnk";
    QFile::remove(lnkPath);
}
bool removeRegistryEntry(QString appName) {
    QString uninstallKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName;

    if (RegDeleteTreeW(HKEY_LOCAL_MACHINE, reinterpret_cast<LPCWSTR>(uninstallKey.utf16())) != ERROR_SUCCESS) {
        return false;
    }
    return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    QString appName = "MyApp"; // TODO: Replace dynamically or pass via args

    // Find Program Files install dir
    QString programFiles = QString::fromWCharArray(_wgetenv(L"ProgramFiles"));
    QString installDir = programFiles + "\\" + appName;

    // Delete shortcuts
    deleteShortcut("Desktop", appName);
    deleteShortcut("StartMenu", appName);

    // Delete files
    QDir dir(installDir);
    if (dir.exists()) {
        dir.removeRecursively();
    }

    // Remove registry entry
    bool regRemoved = removeRegistryEntry(appName);

    QString resultMsg = appName + " has been uninstalled successfully.";
    if (!regRemoved)
        resultMsg += "\n(Note: registry entry may remain).";

    MessageBoxW(NULL, reinterpret_cast<LPCWSTR>(resultMsg.utf16()), L"Uninstall Complete", MB_OK | MB_ICONINFORMATION);

    return 0;
}
    deleteShortcut("Desktop", appName);
    deleteShortcut("StartMenu", appName);
    QFile::remove(installDir + "\\MyApp.exe");
    QFile::remove(installDir + "\\MyApp.dll");
        QFile::remove(installDir + "\\MyApp.pdb");
        QFile::remove(installDir + "\\MyApp.lib");
    QFile::remove(installDir + "\\MyApp.exp");
    QFile::remove(installDir + "\\MyApp.ini");
        QFile::remove(installDir + "\\MyApp.ico");
    QFile::remove(installDir + "\\MyApp.manifest");
    QFile::remove(installDir + "\\MyApp.vshost.exe");
        QFile::remove(installDir + "\\MyApp.vshost.exe.config");
        QFile::remove(installDir + "\\MyApp.vshost.manifest");
    QFile::remove(installDir + "\\Uninstaller.exe");
    QFile::remove(installDir + "\\Uninstaller.exe.config");
    QFile::remove(installDir + "\\Uninstaller.ini");
    QFile::remove(installDir + "\\Uninstaller.pdb");
    QFile::remove(installDir + "\\Uninstaller.lib");
    QFile::remove(installDir + "\\Uninstaller.exp");
    QFile::remove(installDir + "\\Uninstaller.ini");
    QFile::remove(installDir + "\\Uninstaller.ico");
    QFile::remove(installDir + "\\Uninstaller.manifest");
    QFile::remove(installDir + "\\Uninstaller.vshost.exe");
    QFile::remove(installDir + "\\Uninstaller.vshost.exe.config");
    QFile::remove(installDir + "\\Uninstaller.vshost.manifest");
    // Recreate uninstaller to run
    QFile::copy(":/resources/Uninstaller.exe", installDir + "\\Uninstaller.exe");
    QFile::setPermissions(installDir + "\\Uninstaller.exe", QFile::ExeOwner | QFile::ReadOwner | QFile::WriteOwner);
    QString uninstallerPath = installDir + "\\Uninstaller.exe";

    // Delete files
    QDir dir(installDir);
    if (dir.exists()) {
        dir.removeRecursively();
        QFile::remove(installDir + "\\MyApp.exe");
    }
    QString uninstallerPath = installDir + "\\Uninstaller.exe";
    QFile::copy(":/resources/Uninstaller.exe", uninstallerPath);
    QFile::setPermissions(uninstallerPath, QFile::ExeOwner | QFile::ReadOwner | QFile::WriteOwner);
    // Register uninstaller in registry
    HKEY hKey;
    QString uninstallKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + appName;

    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, reinterpret_cast<LPCWSTR>(uninstallKey.utf16()),
                        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        return false;
    }
    QString displayName = appName;
    QString publisher = "MyCompany"; // TODO: Replace with actual publisher
    QString version = "1.0.0"; // TODO: Replace with actual version

    RegSetValueExW(hKey, L"DisplayName", 0, REG_SZ,
                   (BYTE*)displayName.utf16(), (displayName.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"Publisher", 0, REG_SZ,
                   (BYTE*)publisher.utf16(), (publisher.size() + 1) * sizeof(wchar_t));
    RegSetValueExW(hKey, L"DisplayVersion", 0, REG_SZ,
                   (BYTE*)version.utf16(), (version.size() + 1) * sizeof(wchar_t));

    QString uninstallCommand = "\"" + uninstallerPath + "\"";
    RegSetValueExW(hKey, L"UninstallString", 0, REG_SZ,
               (BYTE*)uninstallCommand.utf16(), (uninstallCommand.size() + 1) * sizeof(wchar_t));
    RegCloseKey(hKey);
    // Launch uninstaller
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = L"open";
    sei.lpFile = reinterpret_cast<LPCWSTR>(uninstallerPath.utf16());
    sei.lpParameters = NULL;
    sei.lpDirectory = NULL;
    sei.nShow = SW_SHOWNORMAL;
    sei.hInstApp = NULL;
    if (!ShellExecuteExW(&sei)) {
        return false;
    }
    WaitForSingleObject(sei.hProcess, INFINITE);
    CloseHandle(sei.hProcess);
    // Delete uninstaller
    QFile::remove(uninstallerPath);
    CoUninitialize();

    // Remove registry entry
    bool regRemoved = removeRegistryEntry(appName);
    CoUninitialize();

    QString resultMsg = appName + " has been uninstalled successfully.";
    if (!regRemoved)
        resultMsg += "\n(Note: registry entry may remain).";

    MessageBoxW(NULL, reinterpret_cast<LPCWSTR>(resultMsg.utf16()), L"Uninstall Complete", MB_OK | MB_ICONINFORMATION);
    CoUninitialize();

    return 0;
}
