#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include "src/SimpleBuilder.h"

int main(int argc, char* argv[]) {
    std::cout << "Shuriken Lite - C++ Builder Console Version" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <source_directory>" << std::endl;
        std::cout << "       " << argv[0] << " --help" << std::endl;
        return 1;
    }
    
    std::string sourceDir = argv[1];
    
    if (sourceDir == "--help") {
        std::cout << "Shuriken Lite C++ Builder" << std::endl;
        std::cout << "Features:" << std::endl;
        std::cout << "  - Automatic C++ project compilation" << std::endl;
        std::cout << "  - Installer generation" << std::endl;
        std::cout << "  - Cross-platform builds" << std::endl;
        return 0;
    }
    
    // Check if source directory exists
    if (!std::filesystem::exists(sourceDir)) {
        std::cerr << "Error: Source directory '" << sourceDir << "' not found!" << std::endl;
        return 1;
    }
    
    std::cout << "Building project from: " << sourceDir << std::endl;
    
    // For now, just show what we would do
    std::cout << "Scanning for C++ files..." << std::endl;
    
    std::vector<std::string> cppFiles;
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDir)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension().string();
                if (ext == ".cpp" || ext == ".cxx" || ext == ".cc") {
                    cppFiles.push_back(entry.path().string());
                    std::cout << "  Found: " << entry.path().filename().string() << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
        return 1;
    }
    
    if (cppFiles.empty()) {
        std::cout << "No C++ files found in the directory." << std::endl;
        return 1;
    }
    
    std::cout << "Found " << cppFiles.size() << " C++ file(s)" << std::endl;
    
    // Actually try to build
    SimpleBuilder builder;
    std::string buildLog;
    std::string outputExe = sourceDir + "/output_program";
    
    std::cout << "Starting compilation..." << std::endl;
    bool success = builder.compileProject(sourceDir, outputExe, buildLog);
    
    std::cout << "\n=== Build Log ===" << std::endl;
    std::cout << buildLog << std::endl;
    
    if (success) {
        std::cout << "Build complete! Executable: " << outputExe << std::endl;
    } else {
        std::cout << "Build failed!" << std::endl;
        return 1;
    }
    
    return 0;
}