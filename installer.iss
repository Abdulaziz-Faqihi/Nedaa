; Nidaa - Inno Setup Installer Script

#define MyAppName "Nidaa"
#define MyAppNameAr "نداء"
#define MyAppVersion "1.7.0"
#define MyAppPublisher "Nidaa"
#define MyAppExeName "Nidaa.exe"
#define MyBuildDir "build\Release"

[Setup]
AppId={{B8A3D2E1-7F4C-4A6B-9E5D-1C2F8A3B4D5E}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputDir=installer_output
OutputBaseFilename=NidaaSetup_{#MyAppVersion}
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=lowest
CloseApplications=yes
RestartApplications=no

[Languages]
Name: "arabic"; MessagesFile: "compiler:Languages\Arabic.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "startup"; Description: "{cm:AutoStartDescription}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[CustomMessages]
arabic.AutoStartDescription=تشغيل نداء عند بدء تشغيل Windows
english.AutoStartDescription=Start Nidaa when Windows starts
arabic.CreateDesktopIcon=إنشاء اختصار على سطح المكتب
english.CreateDesktopIcon=Create a desktop shortcut
arabic.AdditionalIcons=خيارات إضافية
english.AdditionalIcons=Additional options

[Files]
Source: "{#MyBuildDir}\Nidaa.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\UniversalSpeech.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\nvdaControllerClient.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\ZDSRAPI.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\wxbase32u_vc_x64_custom.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\wxmsw32u_core_vc_x64_custom.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\jpeg62.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\liblzma.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\libpng16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\pcre2-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\tiff.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\sa_cities.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\sa_regions_lite.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\countries.json"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyBuildDir}\states.json"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "--settings"; Tasks: desktopicon

[Registry]
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "{#MyAppName}"; ValueData: """{app}\{#MyAppExeName}"""; Flags: uninsdeletevalue; Tasks: startup

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchAfterInstall}"; Flags: nowait postinstall skipifsilent

[CustomMessages]
arabic.LaunchAfterInstall=تشغيل نداء الآن
english.LaunchAfterInstall=Launch Nidaa now

[UninstallDelete]
Type: filesandordirs; Name: "{localappdata}\Nidaa"
