#!/bin/bash

# Shuriken Lite - Quick Install Script
# This script downloads and installs the latest version of Shuriken Lite

set -e

echo "🗡️  Shuriken Lite - Quick Installer"
echo "=================================="

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    OS="windows"
else
    echo "❌ Unsupported operating system: $OSTYPE"
    exit 1
fi

echo "🔍 Detected OS: $OS"

# Create install directory
INSTALL_DIR="$HOME/.local/bin"
mkdir -p "$INSTALL_DIR"

echo "📁 Install directory: $INSTALL_DIR"

# For now, we'll build from source since we don't have releases yet
echo "📥 Downloading Shuriken Lite..."
if ! command -v git &> /dev/null; then
    echo "❌ Git is required but not installed. Please install git first."
    exit 1
fi

# Clone repo to temp directory
TEMP_DIR=$(mktemp -d)
cd "$TEMP_DIR"

git clone https://github.com/mllinman/Shuriken_lite.git
cd Shuriken_lite

echo "🔨 Building Shuriken Lite..."

# Check for build dependencies
if ! command -v cmake &> /dev/null; then
    echo "❌ CMake is required. Please install cmake and build tools:"
    if [[ "$OS" == "linux" ]]; then
        echo "   sudo apt install build-essential cmake g++"
    elif [[ "$OS" == "macos" ]]; then
        echo "   brew install cmake gcc"
    fi
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "❌ Make is required. Please install build tools first."
    exit 1
fi

# Build
mkdir -p build
cd build
cmake ..
make -j$(nproc 2>/dev/null || echo 4)

echo "📦 Installing executables..."

# Copy to install directory
cp shuriken_web "$INSTALL_DIR/"
cp shuriken_console "$INSTALL_DIR/"

echo "✅ Installation complete!"
echo ""
echo "🚀 To start Shuriken Lite web interface:"
echo "   $INSTALL_DIR/shuriken_web"
echo ""
echo "🖥️  To use console version:"
echo "   $INSTALL_DIR/shuriken_console /path/to/your/project"
echo ""
echo "💡 Add $INSTALL_DIR to your PATH to use from anywhere:"
echo "   echo 'export PATH=\"\$PATH:$INSTALL_DIR\"' >> ~/.bashrc"
echo "   source ~/.bashrc"
echo ""

# Cleanup
cd /
rm -rf "$TEMP_DIR"

echo "🎉 Happy coding with Shuriken Lite!"