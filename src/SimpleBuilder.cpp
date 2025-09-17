#include "SimpleBuilder.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <memory>

SimpleBuilder::SimpleBuilder(const std::string& toolchainPath) : toolchainPath(toolchainPath) {}

bool SimpleBuilder::compileProject(const std::string& sourceDir, const std::string& outputExe, std::string& log) {
    if (!std::filesystem::exists(sourceDir)) {
        log += "Source directory not found: " + sourceDir + "\n";
        return false;
    }

    // Collect source files
    std::vector<std::string> cppFiles;
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDir)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".cxx" || ext == ".cc" || ext == ".c") {
                    cppFiles.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::exception& e) {
        log += "Error scanning source directory: " + std::string(e.what()) + "\n";
        return false;
    }

    if (cppFiles.empty()) {
        log += "No C++ source files found in: " + sourceDir + "\n";
        return false;
    }

    log += "Found " + std::to_string(cppFiles.size()) + " source file(s)\n";
    for (const auto& file : cppFiles) {
        log += "  " + std::filesystem::path(file).filename().string() + "\n";
    }

    // Build compilation command
    std::vector<std::string> args;
    args.push_back("g++");
    args.push_back("-std=c++17");
    args.push_back("-O2");
    args.push_back("-Wall");
    args.push_back("-o");
    args.push_back(outputExe);
    
    for (const auto& file : cppFiles) {
        args.push_back(file);
    }

    return runCompiler(args, log);
}

bool SimpleBuilder::runCompiler(const std::vector<std::string>& args, std::string& log) {
    // Build command string
    std::string command;
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) command += " ";
        command += "\"" + args[i] + "\"";
    }
    
    log += "Executing: " + command + "\n";

    // Execute command and capture output
    FILE* pipe = popen((command + " 2>&1").c_str(), "r");
    if (!pipe) {
        log += "Failed to execute compiler command\n";
        return false;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        log += buffer;
    }

    int returnCode = pclose(pipe);
    
    if (returnCode == 0) {
        log += "Compilation successful!\n";
    } else {
        log += "Compilation failed with error code: " + std::to_string(returnCode) + "\n";
    }

    return returnCode == 0;
}