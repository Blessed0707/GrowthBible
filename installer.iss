[Setup]
AppName=Growth Bible
AppVersion=1.0
AppPublisher=Blessed0707
AppPublisherURL=https://github.com/Blessed0707/GrowthBible
DefaultDirName={pf}\GrowthBible
DefaultGroupName=Growth Bible
OutputDir=installer_output
OutputBaseFilename=GrowthBibleSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "build\GrowthBibleGUI.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "src\features\sqlite\holybible.db"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Growth Bible"; Filename: "{app}\GrowthBibleGUI.exe"
Name: "{commondesktop}\Growth Bible"; Filename: "{app}\GrowthBibleGUI.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Run]
Filename: "{app}\GrowthBibleGUI.exe"; Description: "Launch Growth Bible"; Flags: nowait postinstall skipifsilent