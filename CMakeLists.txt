cmake_minimum_required(VERSION 3.16)
project(CppBuilderApp VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find all required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Json)


add_executable(CppBuilderApp
    main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/Installer.cpp
    src/Installer.h
    src/Builder.cpp
    src/Builder.h
    src/Uninstaller.cpp
    ui/MainWindow.ui
)


# Link all required Qt6 components
target_link_libraries(CppBuilderApp PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Json)

# --- Installer via CPack ---
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "CppBuilderApp")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "NSIS")
include(CPack)
