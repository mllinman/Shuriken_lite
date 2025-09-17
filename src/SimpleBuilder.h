#pragma once
#include <string>
#include <vector>

class SimpleBuilder {
public:
    SimpleBuilder(const std::string& toolchainPath = "");
    bool compileProject(const std::string& sourceDir, const std::string& outputExe, std::string& log);

private:
    std::string toolchainPath;
    bool runCompiler(const std::vector<std::string>& args, std::string& log);
};