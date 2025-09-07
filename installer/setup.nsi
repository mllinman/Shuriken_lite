!define APPNAME "CppBuilderApp"
!define VERSION "1.0"
OutFile "CppBuilderAppInstaller.exe"
InstallDir $PROGRAMFILES\${APPNAME}
Page directory
Page instfiles

Section "Install"
  SetOutPath $INSTDIR
  File /r "build\Release\CppBuilderApp.exe"
  CreateShortcut "$DESKTOP\CppBuilderApp.lnk" "$INSTDIR\CppBuilderApp.exe"
SectionEnd
