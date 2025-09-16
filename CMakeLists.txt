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

# Add clang include path
include_directories(/usr/lib/llvm-18/include)

add_executable(Shuriken_lite
    main.cpp
    src/MainWindow_simple.cpp
    src/MainWindow_simple.h
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
    src/Debugger.cpp
    src/Debugger.h
    src/CodeEditor_simple.cpp
    src/CodeEditor.h
    src/TerminalWidget.cpp
    src/TerminalWidget.h
    src/Updater.cpp
    src/Updater.h
    src/CodeViewer.cpp
    src/CodeViewer.h
    src/CodeCompleter.cpp
    src/CodeCompleter.h
    ui/Mainwindow.ui
)

qt6_add_resources(Shuriken_lite "resources" PREFIX "/" FILES
    resources/darktheme.qss
    resources/shuriken_logo.png
)
# Link all required Qt6 components

target_link_libraries(Shuriken_lite Qt6::Widgets Qt6::Network Qt6::WebSockets /usr/lib/llvm-18/lib/libclang.so)

# --- Installer via CPack ---
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "Shuriken_lite")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "NSIS")
include(CPack)

if(WIN32)
    set(SHURIKEN_RC ${CMAKE_CURRENT_SOURCE_DIR}/version.rc)
endif()
set_target_properties(Shuriken_lite PROPERTIES OUTPUT_NAME "Shuriken_lite")
install(TARGETS Shuriken_lite DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION share/doc)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE DESTINATION .)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/plugins DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/themes DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ui DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION .)
# --- End of Installer via CPack ---