#include <iostream>
#include <string>
#include <filesystem>
#include <thread>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>
#include "src/SimpleBuilder.h"
#include "src/SimpleInstaller.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

class WebServer {
private:
    SimpleBuilder builder;
    SimpleInstaller installer;
    int port;
    
    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    std::string getMimeType(const std::string& path) {
        if (path.size() >= 5 && path.substr(path.size()-5) == ".html") return "text/html";
        if (path.size() >= 4 && path.substr(path.size()-4) == ".css") return "text/css";
        if (path.size() >= 3 && path.substr(path.size()-3) == ".js") return "application/javascript";
        if (path.size() >= 5 && path.substr(path.size()-5) == ".json") return "application/json";
        return "text/plain";
    }
    
    std::string urlDecode(const std::string& str) {
        std::string decoded;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '%' && i + 2 < str.length()) {
                int hex = std::stoi(str.substr(i + 1, 2), nullptr, 16);
                decoded += static_cast<char>(hex);
                i += 2;
            } else if (str[i] == '+') {
                decoded += ' ';
            } else {
                decoded += str[i];
            }
        }
        return decoded;
    }
    
    std::map<std::string, std::string> parseQueryString(const std::string& query) {
        std::map<std::string, std::string> params;
        std::stringstream ss(query);
        std::string param;
        
        while (std::getline(ss, param, '&')) {
            size_t eq = param.find('=');
            if (eq != std::string::npos) {
                std::string key = urlDecode(param.substr(0, eq));
                std::string value = urlDecode(param.substr(eq + 1));
                params[key] = value;
            }
        }
        return params;
    }
    
    std::string handleBuildRequest(const std::string& projectPath) {
        if (!std::filesystem::exists(projectPath)) {
            return R"({"success": false, "error": "Project path does not exist"})";
        }
        
        std::string buildLog;
        std::string outputExe = projectPath + "/ShurikenBuild";
        bool success = builder.compileProject(projectPath, outputExe, buildLog);
        
        // Escape JSON strings
        std::regex newline("\\n");
        std::regex quote("\"");
        buildLog = std::regex_replace(buildLog, quote, "\\\"");
        buildLog = std::regex_replace(buildLog, newline, "\\n");
        
        std::ostringstream response;
        response << "{";
        response << "\"success\": " << (success ? "true" : "false") << ",";
        response << "\"log\": \"" << buildLog << "\",";
        response << "\"executable\": \"" << outputExe << "\"";
        response << "}";
        
        return response.str();
    }
    
    std::string handleInstallerRequest(const std::string& executable, const std::string& installerType) {
        if (!std::filesystem::exists(executable)) {
            return R"({"success": false, "error": "Executable not found"})";
        }
        
        std::string installerLog;
        std::string outputPath = std::filesystem::path(executable).parent_path().string() + "/installer_output";
        
        bool success = false;
        if (installerType == "windows") {
            success = installer.createInstaller(executable, outputPath + ".exe", installerLog);
        } else if (installerType == "debian") {
            success = installer.createDebPackage(executable, outputPath, installerLog);
        } else if (installerType == "portable") {
            success = installer.createPortablePackage(executable, outputPath, installerLog);
        }
        
        // Escape JSON strings
        std::regex newline("\\n");
        std::regex quote("\"");
        installerLog = std::regex_replace(installerLog, quote, "\\\"");
        installerLog = std::regex_replace(installerLog, newline, "\\n");
        
        std::ostringstream response;
        response << "{";
        response << "\"success\": " << (success ? "true" : "false") << ",";
        response << "\"log\": \"" << installerLog << "\",";
        response << "\"type\": \"" << installerType << "\",";
        response << "\"output\": \"" << outputPath << "\"";
        response << "}";
        
        return response.str();
    }
    
    std::string createHttpResponse(int status, const std::string& contentType, const std::string& body) {
        std::ostringstream response;
        response << "HTTP/1.1 " << status << " ";
        response << (status == 200 ? "OK" : "Not Found") << "\r\n";
        response << "Content-Type: " << contentType << "\r\n";
        response << "Content-Length: " << body.length() << "\r\n";
        response << "Access-Control-Allow-Origin: *\r\n";
        response << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
        response << "Access-Control-Allow-Headers: Content-Type\r\n";
        response << "\r\n";
        response << body;
        return response.str();
    }

public:
    WebServer(int port = 8080) : port(port), installer("MyApp", "1.0.0") {}
    
    void start() {
        std::cout << "🌐 Starting Shuriken Lite Web Server..." << std::endl;
        
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }
        
        int opt = 1;
#ifdef _WIN32
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
        
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Failed to bind to port " << port << std::endl;
#ifdef _WIN32
            closesocket(server_fd);
            WSACleanup();
#else
            close(server_fd);
#endif
            return;
        }
        
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Failed to listen" << std::endl;
#ifdef _WIN32
            closesocket(server_fd);
            WSACleanup();
#else
            close(server_fd);
#endif
            return;
        }
        
        std::cout << "🚀 Server running at http://localhost:" << port << std::endl;
        std::cout << "📱 Open your web browser and navigate to the above URL" << std::endl;
        std::cout << "🔧 IDE interface ready for C++ development!" << std::endl;
        
        while (true) {
            sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
#ifdef _WIN32
            int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
#else
            int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
#endif
            
            if (client_fd < 0) {
                continue;
            }
            
            // Handle request in a new thread
            std::thread([this, client_fd]() {
                this->handleRequest(client_fd);
            }).detach();
        }
        
#ifdef _WIN32
        closesocket(server_fd);
        WSACleanup();
#else
        close(server_fd);
#endif
    }
    
private:
    void handleRequest(int client_fd) {
        char buffer[4096] = {0};
        recv(client_fd, buffer, sizeof(buffer), 0);
        
        std::string request(buffer);
        std::stringstream ss(request);
        std::string method, path, version;
        ss >> method >> path >> version;
        
        std::string response;
        
        if (method == "OPTIONS") {
            response = createHttpResponse(200, "text/plain", "");
        } else if (path == "/" || path == "/index.html") {
            std::string content = readFile("web_ui/index.html");
            if (!content.empty()) {
                response = createHttpResponse(200, "text/html", content);
            } else {
                response = createHttpResponse(404, "text/plain", "File not found");
            }
        } else if (path.substr(0, 10) == "/api/build") {
            if (method == "POST") {
                // Parse project path from POST data
                size_t bodyStart = request.find("\r\n\r\n");
                if (bodyStart != std::string::npos) {
                    std::string body = request.substr(bodyStart + 4);
                    auto params = parseQueryString(body);
                    
                    if (params.count("projectPath")) {
                        std::string result = handleBuildRequest(params["projectPath"]);
                        response = createHttpResponse(200, "application/json", result);
                    } else {
                        response = createHttpResponse(400, "application/json", 
                                                   R"({"success": false, "error": "Missing projectPath parameter"})");
                    }
                } else {
                    response = createHttpResponse(400, "application/json", 
                                               R"({"success": false, "error": "Invalid request body"})");
                }
            } else {
                response = createHttpResponse(405, "text/plain", "Method not allowed");
            }
        } else if (path.substr(0, 14) == "/api/installer") {
            if (method == "POST") {
                size_t bodyStart = request.find("\r\n\r\n");
                if (bodyStart != std::string::npos) {
                    std::string body = request.substr(bodyStart + 4);
                    auto params = parseQueryString(body);
                    
                    if (params.count("executable") && params.count("type")) {
                        std::string result = handleInstallerRequest(params["executable"], params["type"]);
                        response = createHttpResponse(200, "application/json", result);
                    } else {
                        response = createHttpResponse(400, "application/json", 
                                                   R"({"success": false, "error": "Missing parameters"})");
                    }
                } else {
                    response = createHttpResponse(400, "application/json", 
                                               R"({"success": false, "error": "Invalid request body"})");
                }
            } else {
                response = createHttpResponse(405, "text/plain", "Method not allowed");
            }
        } else {
            response = createHttpResponse(404, "text/plain", "Not Found");
        }
        
        send(client_fd, response.c_str(), response.length(), 0);
        
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
    }
};

int main(int argc, char* argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }
    
    std::cout << "🗡️  Shuriken Lite - Modern C++ IDE & Builder" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    WebServer server(port);
    server.start();
    
    return 0;
}