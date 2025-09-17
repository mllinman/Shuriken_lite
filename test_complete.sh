#!/bin/bash

# Shuriken Lite - Complete Build and Test Script
echo "🗡️  Shuriken Lite - Modern C++ IDE"
echo "=================================="
echo

# Build the project
echo "🔨 Building Shuriken Lite..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "✅ Build successful!"
echo

# Test console version
echo "🧪 Testing console version..."
../build/shuriken_console --help

# Create test project
echo "📁 Creating test project..."
mkdir -p /tmp/shuriken_test
cat > /tmp/shuriken_test/hello.cpp << 'EOF'
#include <iostream>
#include <string>

int main() {
    std::string name = "Shuriken Lite";
    std::cout << "Hello from " << name << "!" << std::endl;
    std::cout << "Modern C++ IDE with beautiful interface." << std::endl;
    return 0;
}
EOF

# Test build
echo "🔧 Testing C++ compilation..."
../build/shuriken_console /tmp/shuriken_test

if [ -f "/tmp/shuriken_test/output_program" ]; then
    echo "✅ Compilation test passed!"
    echo "🚀 Testing generated executable..."
    /tmp/shuriken_test/output_program
    echo
else
    echo "❌ Compilation test failed!"
    exit 1
fi

# Start web server in background for testing
echo "🌐 Starting web server..."
./shuriken_web 8080 &
SERVER_PID=$!
sleep 3

# Test web interface
echo "🌍 Testing web interface..."
if curl -s http://localhost:8080 | grep -q "Shuriken Lite"; then
    echo "✅ Web interface is working!"
else
    echo "❌ Web interface test failed!"
    kill $SERVER_PID 2>/dev/null
    exit 1
fi

# Test API
echo "🔌 Testing build API..."
API_RESPONSE=$(curl -s -X POST http://localhost:8080/api/build \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -d "projectPath=/tmp/shuriken_test")

if echo "$API_RESPONSE" | grep -q '"success": true'; then
    echo "✅ Build API is working!"
else
    echo "❌ Build API test failed!"
    echo "Response: $API_RESPONSE"
    kill $SERVER_PID 2>/dev/null
    exit 1
fi

# Test installer API
echo "📦 Testing installer API..."
INSTALLER_RESPONSE=$(curl -s -X POST http://localhost:8080/api/installer \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -d "executable=/tmp/shuriken_test/output_program&type=portable")

if echo "$INSTALLER_RESPONSE" | grep -q '"success": true'; then
    echo "✅ Installer API is working!"
else
    echo "❌ Installer API test failed!"
    echo "Response: $INSTALLER_RESPONSE"
    kill $SERVER_PID 2>/dev/null
    exit 1
fi

# Clean up
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

echo
echo "🎉 All tests passed!"
echo "✨ Shuriken Lite is ready for use!"
echo
echo "Usage:"
echo "  Console: ./build/shuriken_console <project_path>"
echo "  Web IDE: ./build/shuriken_web [port] (default: 8080)"
echo
echo "🌐 Open http://localhost:8080 for the beautiful web interface!"