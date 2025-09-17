#pragma once
#include <string>
#include <vector>

class SimpleInstaller {
public:
    SimpleInstaller(const std::string& appName, const std::string& version = "1.0.0");
    bool createInstaller(const std::string& executable, const std::string& outputPath, std::string& log);
    bool createPortablePackage(const std::string& executable, const std::string& outputPath, std::string& log);
    bool createDebPackage(const std::string& executable, const std::string& outputPath, std::string& log);
    
private:
    std::string appName;
    std::string version;
    std::string description;
    
    bool copyFile(const std::string& source, const std::string& dest);
    bool createDirectory(const std::string& path);
    bool generateNSISScript(const std::string& executable, const std::string& scriptPath);
    bool generateDebianControl(const std::string& controlPath);
    bool executeCommand(const std::string& command, std::string& output);
};