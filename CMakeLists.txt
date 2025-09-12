cmake_minimum_required(VERSION 3.16)
project(Shuriken_lite VERSION 1.0 LANGUAGES CXX)


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
find_package(Qt6 REQUIRED COMPONENTS Widgets Network WebSockets)

add_executable(Shuriken_lite
    main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/Installer.cpp
    src/Installer.h
    src/Builder.cpp
    src/Builder.h
    src/Uninstaller.cpp
    src/Uninstaller.h
    src/NotificationManager.cpp
    src/NotificationManager.h
    src/ProjectHubPanel.cpp
    src/ProjectHubPanel.h
    src/IssueManager.cpp
    src/IssueManager.h
    ui/MainWindow.ui
)

# Explicitly associate the .ui file with the target
target_sources(Shuriken_lite PRIVATE ui/MainWindow.ui)

qt6_add_resources(RESOURCES resources.qrc)

# Removed duplicate add_executable to fix target redefinition error
# Link all required Qt6 components
target_link_libraries(Shuriken_lite Qt6::Widgets Qt6::Network Qt6::WebSockets)

# --- Installer via CPack ---
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "Shuriken_lite")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "NSIS")
include(CPack)
