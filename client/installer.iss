[Setup]
AppName=Shuriken_lite
AppPublisher=BulletDrop_Studio
AppPublisherURL=https://bulletsdropstudio.com
AppVersion=1.0
DefaultDirName={pf}\Shuriken_lite
DefaultGroupName=Shuriken_lite
OutputDir=dist
OutputBaseFilename=Shuriken_liteInstaller
Compression=lzma
SolidCompression=yes

[Files]
Source: "build\Shuriken_lite.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Shuriken_lite"; Filename: "{app}\Shuriken_lite.exe"
