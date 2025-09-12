cmake_minimum_required(VERSION 3.16)
project(CppBuilderApp VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Explicitly set the UI search path for AutoUic
set(CMAKE_AUTOUIC_SEARCH_PATH "ui")

# Add ui directory to include directories
include_directories(${CMAKE_SOURCE_DIR}/ui)


# Find all required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebSockets)


add_executable(CppBuilderApp
    main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/Installer.cpp
    src/Installer.h
    src/Builder.cpp
    src/Builder.h
    src/Uninstaller.cpp
)

# Explicitly associate the .ui file with the target
target_sources(CppBuilderApp PRIVATE ui/MainWindow.ui)



# Link all required Qt6 components
target_link_libraries(CppBuilderApp PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WebSockets)

# --- Installer via CPack ---
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "CppBuilderApp")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "NSIS")
include(CPack)
