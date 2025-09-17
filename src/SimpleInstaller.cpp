#include "SimpleInstaller.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>

SimpleInstaller::SimpleInstaller(const std::string& appName, const std::string& version)
    : appName(appName), version(version), description(appName + " - Built with Shuriken Lite") {}

bool SimpleInstaller::createInstaller(const std::string& executable, const std::string& outputPath, std::string& log) {
    if (!std::filesystem::exists(executable)) {
        log += "Executable not found: " + executable + "\n";
        return false;
    }
    
    log += "Creating Windows installer package...\n";
    log += "Application: " + appName + " v" + version + "\n";
    
    // Create temporary directory for installer files
    std::string tempDir = "/tmp/shuriken_installer_" + appName;
    if (!createDirectory(tempDir)) {
        log += "Failed to create temporary directory\n";
        return false;
    }
    
    // Copy executable to temp directory
    std::string targetExe = tempDir + "/" + appName + ".exe";
    if (!copyFile(executable, targetExe)) {
        log += "Failed to copy executable\n";
        return false;
    }
    
    // Generate NSIS installer script
    std::string scriptPath = tempDir + "/installer.nsi";
    if (!generateNSISScript(targetExe, scriptPath)) {
        log += "Failed to generate installer script\n";
        return false;
    }
    
    log += "✅ Installer package prepared!\n";
    log += "Location: " + outputPath + "\n";
    log += "Note: NSIS compiler required to build Windows installer\n";
    
    return true;
}

bool SimpleInstaller::createPortablePackage(const std::string& executable, const std::string& outputPath, std::string& log) {
    log += "Creating portable package...\n";
    
    std::string packageDir = outputPath + "/" + appName + "_v" + version + "_portable";
    if (!createDirectory(packageDir)) {
        log += "Failed to create package directory\n";
        return false;
    }
    
    // Copy executable
    std::string targetExe = packageDir + "/" + appName;
    if (!copyFile(executable, targetExe)) {
        log += "Failed to copy executable\n";
        return false;
    }
    
    // Create README
    std::string readmePath = packageDir + "/README.txt";
    std::ofstream readme(readmePath);
    readme << appName << " v" << version << "\n";
    readme << "Built with Shuriken Lite IDE\n\n";
    readme << "This is a portable version - no installation required.\n";
    readme << "Simply run the " << appName << " executable.\n\n";
    readme << "For support and updates, visit: https://github.com/mllinman/Shuriken_lite\n";
    readme.close();
    
    // Create run script
    std::string runScript = packageDir + "/run.sh";
    std::ofstream script(runScript);
    script << "#!/bin/bash\n";
    script << "cd \"$(dirname \"$0\")\"\n";
    script << "./" << appName << "\n";
    script.close();
    
    // Make executable
    std::filesystem::permissions(runScript, std::filesystem::perms::owner_exec | 
                                std::filesystem::perms::group_exec | 
                                std::filesystem::perms::others_exec, 
                                std::filesystem::perm_options::add);
    
    log += "✅ Portable package created!\n";
    log += "Location: " + packageDir + "\n";
    
    return true;
}

bool SimpleInstaller::createDebPackage(const std::string& executable, const std::string& outputPath, std::string& log) {
    log += "Creating Debian package...\n";
    
    std::string debDir = outputPath + "/" + appName + "_" + version + "_deb";
    std::string debianDir = debDir + "/DEBIAN";
    std::string binDir = debDir + "/usr/local/bin";
    
    if (!createDirectory(debianDir) || !createDirectory(binDir)) {
        log += "Failed to create package directories\n";
        return false;
    }
    
    // Copy executable to bin directory
    std::string targetExe = binDir + "/" + appName;
    if (!copyFile(executable, targetExe)) {
        log += "Failed to copy executable\n";
        return false;
    }
    
    // Generate control file
    if (!generateDebianControl(debianDir + "/control")) {
        log += "Failed to generate control file\n";
        return false;
    }
    
    // Create postinst script
    std::string postinst = debianDir + "/postinst";
    std::ofstream postScript(postinst);
    postScript << "#!/bin/bash\n";
    postScript << "chmod +x /usr/local/bin/" << appName << "\n";
    postScript.close();
    
    std::filesystem::permissions(postinst, std::filesystem::perms::owner_exec, 
                                std::filesystem::perm_options::add);
    
    log += "✅ Debian package structure created!\n";
    log += "Location: " + debDir + "\n";
    log += "Note: Run 'dpkg-deb --build " + debDir + "' to create final .deb package\n";
    
    return true;
}

bool SimpleInstaller::copyFile(const std::string& source, const std::string& dest) {
    try {
        std::filesystem::copy_file(source, dest, std::filesystem::copy_options::overwrite_existing);
        std::filesystem::permissions(dest, std::filesystem::perms::owner_exec | 
                                    std::filesystem::perms::group_exec | 
                                    std::filesystem::perms::others_exec, 
                                    std::filesystem::perm_options::add);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool SimpleInstaller::createDirectory(const std::string& path) {
    try {
        std::filesystem::create_directories(path);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool SimpleInstaller::generateNSISScript(const std::string& executable, const std::string& scriptPath) {
    std::ofstream script(scriptPath);
    if (!script.is_open()) return false;
    
    script << ";NSIS Installer Script for " << appName << "\n";
    script << "!define APPNAME \"" << appName << "\"\n";
    script << "!define VERSION \"" << version << "\"\n";
    script << "!define DESCRIPTION \"" << description << "\"\n\n";
    
    script << "Name \"${APPNAME} ${VERSION}\"\n";
    script << "OutFile \"" << appName << "_Setup_${VERSION}.exe\"\n";
    script << "InstallDir \"$PROGRAMFILES\\${APPNAME}\"\n\n";
    
    script << "Page directory\n";
    script << "Page instfiles\n\n";
    
    script << "Section \"Install\"\n";
    script << "  SetOutPath \"$INSTDIR\"\n";
    script << "  File \"" << std::filesystem::path(executable).filename().string() << "\"\n";
    script << "  CreateShortcut \"$DESKTOP\\${APPNAME}.lnk\" \"$INSTDIR\\" << std::filesystem::path(executable).filename().string() << "\"\n";
    script << "  CreateDirectory \"$SMPROGRAMS\\${APPNAME}\"\n";
    script << "  CreateShortcut \"$SMPROGRAMS\\${APPNAME}\\${APPNAME}.lnk\" \"$INSTDIR\\" << std::filesystem::path(executable).filename().string() << "\"\n";
    script << "  CreateShortcut \"$SMPROGRAMS\\${APPNAME}\\Uninstall.lnk\" \"$INSTDIR\\Uninstall.exe\"\n";
    script << "  WriteUninstaller \"$INSTDIR\\Uninstall.exe\"\n";
    script << "SectionEnd\n\n";
    
    script << "Section \"Uninstall\"\n";
    script << "  Delete \"$INSTDIR\\" << std::filesystem::path(executable).filename().string() << "\"\n";
    script << "  Delete \"$INSTDIR\\Uninstall.exe\"\n";
    script << "  RMDir \"$INSTDIR\"\n";
    script << "  Delete \"$DESKTOP\\${APPNAME}.lnk\"\n";
    script << "  Delete \"$SMPROGRAMS\\${APPNAME}\\${APPNAME}.lnk\"\n";
    script << "  Delete \"$SMPROGRAMS\\${APPNAME}\\Uninstall.lnk\"\n";
    script << "  RMDir \"$SMPROGRAMS\\${APPNAME}\"\n";
    script << "SectionEnd\n";
    
    script.close();
    return true;
}

bool SimpleInstaller::generateDebianControl(const std::string& controlPath) {
    std::ofstream control(controlPath);
    if (!control.is_open()) return false;
    
    control << "Package: " << appName << "\n";
    control << "Version: " << version << "\n";
    control << "Architecture: amd64\n";
    control << "Maintainer: Shuriken Lite <noreply@shurikenlite.com>\n";
    control << "Description: " << description << "\n";
    control << " " << description << "\n";
    control << " Built using Shuriken Lite IDE - Modern C++ Development Environment\n";
    control << "Section: devel\n";
    control << "Priority: optional\n";
    control << "Depends: libc6 (>= 2.31)\n";
    
    control.close();
    return true;
}

bool SimpleInstaller::executeCommand(const std::string& command, std::string& output) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return false;
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    
    int result = pclose(pipe);
    return result == 0;
}