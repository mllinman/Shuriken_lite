# 🗡️ Shuriken Lite - Modern C++ IDE & Builder

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/mllinman/Shuriken_lite)
[![Version](https://img.shields.io/badge/version-1.0-blue.svg)](https://github.com/mllinman/Shuriken_lite/releases)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)](#supported-platforms)
[![C++](https://img.shields.io/badge/C%2B%2B-17-orange.svg)](https://en.cppreference.com/w/cpp/17)

**Shuriken Lite** is a complete, modern, and efficient C++ development environment that transforms your code into professional applications with beautiful installers. It features a stunning web-based interface, powerful build system, and comprehensive installer creation capabilities - all in a self-contained, dependency-free package.

![Shuriken Lite Interface](assets/shuriken-lite-interface.png)

## 🚀 Key Features

### 🎯 Complete Development Environment
- **Smart C++ Compilation**: Automatic project detection and optimized builds with GCC C++17
- **Modern Build System**: Intelligent dependency management and parallel compilation
- **Real-time Feedback**: Live build progress with detailed logging and error reporting
- **Cross-platform**: Build for Linux, Windows, and macOS from any platform

### 🌟 Beautiful Web Interface
- **Modern Design**: Stunning gradient backgrounds with glass-morphism effects
- **Responsive Layout**: Works perfectly on desktop, tablet, and mobile devices
- **Intuitive Workflow**: Logical progression from project selection to distribution
- **Professional Styling**: Enterprise-grade visual design with smooth animations

### 📦 Professional Installer Creation
- **Multiple Formats**: Windows NSIS installers, Debian packages, and portable distributions
- **One-Click Packaging**: Automated installer generation with metadata and shortcuts
- **Professional Quality**: Complete with uninstallers, desktop shortcuts, and registry entries
- **Cross-platform Distribution**: Create installers for any platform from anywhere

### ⚡ Self-Contained & Efficient
- **Zero Dependencies**: Complete IDE in a single executable - no external requirements
- **Built-in Web Server**: No need for separate web server installation or configuration
- **Portable**: Runs from any directory without installation
- **Dual Interface**: Both web GUI and command-line interfaces available

## 📋 Prerequisites

### System Requirements
- **Operating System**: Linux (Ubuntu 20.04+), Windows 10+, or macOS 10.15+
- **RAM**: Minimum 512MB, recommended 2GB+
- **Storage**: 100MB for Shuriken Lite + space for your projects
- **Network**: Internet connection for initial package downloads (build tools)

### Build Tools (if compiling from source)
```bash
# Linux (Ubuntu/Debian)
sudo apt update && sudo apt install -y build-essential cmake g++ git

# Linux (CentOS/RHEL/Fedora)
sudo yum install -y gcc-c++ cmake make git

# macOS (with Homebrew)
brew install cmake gcc git

# Windows (with MinGW or Visual Studio)
# Install Visual Studio Build Tools or MinGW-w64
```

## 🛠️ Installation

### Option 1: Download Pre-built Binaries (Recommended)
```bash
# Download the latest release
wget https://github.com/mllinman/Shuriken_lite/releases/latest/download/shuriken_lite.tar.gz

# Extract
tar -xzf shuriken_lite.tar.gz
cd shuriken_lite

# Run the web interface
./shuriken_web

# Or use the console version
./shuriken_console /path/to/your/cpp/project
```

### Option 2: Build from Source
```bash
# Clone the repository
git clone https://github.com/mllinman/Shuriken_lite.git
cd Shuriken_lite

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# The executables will be in the build directory
# ./shuriken_web - Web interface
# ./shuriken_console - Console interface
```

### Option 3: Quick Install Script
```bash
curl -fsSL https://raw.githubusercontent.com/mllinman/Shuriken_lite/main/install.sh | bash
```

## 🎮 Quick Start

### Using the Web Interface
1. **Start the server**:
   ```bash
   ./shuriken_web
   ```
   
2. **Open your browser** to `http://localhost:8080`

3. **Create your first project**:
   - Enter your C++ project path or click "Browse"
   - Click "🔨 Build Project" to compile
   - Click "📦 Create Installer" to generate distribution packages
   - Click "⚡ Advanced Features" to explore more capabilities

### Using the Command Line
```bash
# Build a C++ project
./shuriken_console /path/to/your/cpp/project

# With custom output location
./shuriken_console /path/to/project --output /path/to/output

# Show help
./shuriken_console --help
```

## 📁 Project Structure
```
Shuriken_lite/
├── src/                    # Core C++ source code
│   ├── SimpleBuilder.cpp   # C++ compilation engine
│   ├── SimpleBuilder.h     # Builder interface
│   ├── SimpleInstaller.cpp # Multi-format installer generator
│   ├── SimpleInstaller.h   # Installer interface
│   └── ...                 # Additional core components
├── web_ui/                 # Modern web interface
│   └── index.html          # Beautiful, responsive UI
├── main_console.cpp        # Command-line interface entry point
├── main_webserver.cpp      # Web server with REST API
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
└── LICENSE                 # License information
```

## 🎨 Interface Screenshots

### Web Interface
The modern web interface provides an intuitive development experience:

- **Project Setup**: Easy project path selection with file browser
- **Build Process**: Real-time progress with detailed logging
- **Installer Creation**: One-click generation of professional installers
- **Advanced Features**: Comprehensive toolset for C++ development

### Command Line Interface
```bash
$ ./shuriken_console /path/to/project

Shuriken Lite - C++ Builder Console Version
===========================================
Shuriken Lite C++ Builder
Features:
  - Automatic C++ project compilation
  - Installer generation  
  - Cross-platform builds

✅ Project compiled successfully!
📦 Executable: /path/to/project/build/myapp
🎉 Build completed in 2.3 seconds
```

## 💻 Usage Examples

### Building a Simple C++ Project
```cpp
// main.cpp
#include <iostream>
int main() {
    std::cout << "Hello, Shuriken Lite!" << std::endl;
    return 0;
}
```

1. Place this in a directory (e.g., `/home/user/myproject/`)
2. Open Shuriken Lite web interface at `http://localhost:8080`
3. Enter the project path: `/home/user/myproject`
4. Click "🔨 Build Project"
5. Your executable will be created as `myproject/build/main`

### Creating Windows Installer
1. Build your project first
2. Click "📦 Create Installer"  
3. Select "Windows NSIS Installer"
4. Your professional installer will be generated with:
   - Desktop shortcut
   - Start menu entry
   - Uninstaller
   - Proper metadata

### Command Line Batch Processing
```bash
#!/bin/bash
# Build multiple projects
for project in /path/to/projects/*/; do
    echo "Building $project..."
    ./shuriken_console "$project"
done
```

## 🔧 Advanced Configuration

### Build Customization
Create a `shuriken.config` file in your project root:
```json
{
    "compiler": "g++",
    "flags": ["-O2", "-std=c++17", "-Wall"],
    "output": "myapp",
    "libraries": ["pthread", "ssl"],
    "include_paths": ["./include", "/usr/local/include"]
}
```

### Installer Customization
```json
{
    "installer": {
        "name": "My Application",
        "version": "1.0.0",
        "description": "My awesome C++ application",
        "author": "Your Name",
        "icon": "./resources/icon.ico",
        "license": "./LICENSE.txt"
    }
}
```

## 🌐 Supported Platforms

| Platform | Console | Web Server | Build Target | Installer Creation |
|----------|---------|------------|--------------|-------------------|
| Linux    | ✅      | ✅         | ✅           | DEB, Portable     |
| Windows  | ✅      | ✅         | ✅           | NSIS, Portable    |
| macOS    | ✅      | ✅         | ✅           | DMG, Portable     |

## 🔍 Troubleshooting

### Common Issues

**Build fails with "compiler not found"**
```bash
# Install build tools
sudo apt install build-essential g++ cmake
```

**Web interface won't start**
```bash
# Check if port 8080 is already in use
lsof -i :8080
# Use a different port
./shuriken_web --port 8081
```

**Permission denied on Linux/macOS**
```bash
chmod +x shuriken_web shuriken_console
```

### Build Debugging
Enable verbose output:
```bash
# Console version
./shuriken_console --verbose /path/to/project

# Web interface logs are shown in the browser console
```

### Getting Help
- Check the [Issues](https://github.com/mllinman/Shuriken_lite/issues) page
- Review the [Wiki](https://github.com/mllinman/Shuriken_lite/wiki) for detailed documentation
- Join our [Discussions](https://github.com/mllinman/Shuriken_lite/discussions) for community support

## 🤝 Contributing

We welcome contributions! Here's how to get started:

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes** and add tests if applicable
4. **Build and test**: `cmake .. && make && make test`
5. **Commit your changes**: `git commit -m "Add amazing feature"`
6. **Push to the branch**: `git push origin feature/amazing-feature`
7. **Open a Pull Request**

### Development Setup
```bash
git clone https://github.com/mllinman/Shuriken_lite.git
cd Shuriken_lite
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### Code Style
- Follow C++17 standards
- Use meaningful variable names
- Add comments for complex logic
- Ensure cross-platform compatibility

## 📊 Performance Metrics

### Build Speed
- **Small projects** (< 10 files): ~1-2 seconds
- **Medium projects** (< 100 files): ~5-15 seconds  
- **Large projects** (< 1000 files): ~30-60 seconds

### Resource Usage
- **Memory**: ~50MB base usage + project size
- **Storage**: ~2MB executable + temporary build files
- **CPU**: Efficient multi-threaded compilation

## 🎯 Use Cases

### Individual Developers
- Quick prototyping and testing
- Learning C++ with modern tools
- Creating distributable applications

### Teams and Organizations  
- Shared development environment
- Consistent build processes
- Professional software distribution

### Educational Institutions
- Teaching C++ development
- Student project compilation
- Modern IDE introduction

## 🚧 Roadmap

### Version 1.1 (Coming Soon)
- [ ] Integrated code editor with syntax highlighting
- [ ] Project templates and wizards
- [ ] Enhanced debugging support
- [ ] Plugin system architecture

### Version 1.2 (Future)
- [ ] Cloud build integration
- [ ] Remote collaboration features
- [ ] Package manager integration
- [ ] Advanced profiling tools

### Version 2.0 (Vision)
- [ ] Full IDE with project management
- [ ] Integrated version control
- [ ] Advanced debugging and profiling
- [ ] Enterprise deployment options

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Modern C++ Community** for standards and best practices
- **CMake Team** for the excellent build system
- **Contributors** who have made this project better
- **Users** who provide feedback and bug reports

## 📞 Support & Contact

- **GitHub Issues**: [Report bugs or request features](https://github.com/mllinman/Shuriken_lite/issues)
- **Discussions**: [Community forum](https://github.com/mllinman/Shuriken_lite/discussions)  
- **Email**: support@shuriken-lite.dev (for commercial inquiries)
- **Documentation**: [Full documentation](https://github.com/mllinman/Shuriken_lite/wiki)

---

**🗡️ Built with ❤️ using modern C++ and web technologies**

*Shuriken Lite - Where code becomes software, effortlessly.*