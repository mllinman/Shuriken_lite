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
    QString desc = "Shortcut for " + name;
    psl->SetDescription(reinterpret_cast<LPCWSTR>(desc.utf16()));

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

    // Add missing closing brace for wWinMain
    // ...existing code...
    return 0;
}

