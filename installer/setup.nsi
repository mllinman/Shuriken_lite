!define APPNAME "Shuriken_lite"
!define VERSION "1.0"
OutFile "Shuriken_liteInstaller.exe"
InstallDir $PROGRAMFILES\${APPNAME}
Page directory
Page instfiles

Section "Install"
  SetOutPath $INSTDIR
  File /r "build\Release\Shuriken_lite.exe"
  CreateShortcut "$DESKTOP\Shuriken_lite.lnk" "$INSTDIR\Shuriken_lite.exe"
SectionEnd
