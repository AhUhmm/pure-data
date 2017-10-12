; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Pd-ceammc"
#define MyAppVersion "${CEAMMC_DISTRIB_VERSION}"
#define MyAppPublisher "ceammc"
#define MyAppURL "https://github.com/uliss/pure-data"
#define MyAppExeName "bin/pd.exe"
#define AppRoot "${INSTALLED_BIN_DIR}"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{F480F394-5E58-47F7-A329-BD83FA4B953B}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
OutputDir=inno
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#AppRoot}/bin/*.exe"; DestDir: "{app}/bin"; Flags: ignoreversion
Source: "{#AppRoot}/bin/*.dll"; DestDir: "{app}/bin"; Flags: ignoreversion
Source: "{#AppRoot}/README.txt"; DestDir: "{app}";
Source: "{#AppRoot}/LICENSE.txt"; DestDir: "{app}";
Source: "{#AppRoot}/po/*.msg"; DestDir: "{app}/po";
Source: "{#AppRoot}/tcl/*.tcl"; DestDir: "{app}/tcl"; Flags: ignoreversion recursesubdirs
; completion files
Source: "{#AppRoot}/tcl/*.txt"; DestDir: "{app}/tcl"; Flags: ignoreversion recursesubdirs
Source: "{#AppRoot}/tcl/pd.ico"; DestDir: "{app}/tcl";
Source: "{#AppRoot}/tcl/pd_ceammc.ico"; DestDir: "{app}/tcl";
Source: "{#AppRoot}/lib/*"; DestDir: "{app}/lib"; Flags: ignoreversion recursesubdirs
Source: "{#AppRoot}/extra/*"; Excludes: "*.a"; DestDir: "{app}/extra"; Flags: ignoreversion recursesubdirs
Source: "{#AppRoot}/doc/*"; DestDir: "{app}/doc"; Flags: ignoreversion recursesubdirs
Source: "{#AppRoot}/include/*.h"; DestDir: "{app}/include";
; Source: "{#AppRoot}/fonts/DejaVuSerif.ttf"; DestDir: "{fonts}"; FontInstall: "DejaVu Serif"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "{#AppRoot}/fonts/DejaVuSansMono.ttf"; DestDir: "{fonts}"; FontInstall: "DejaVu Sans Mono"; Flags: onlyifdoesntexist uninsneveruninstall

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\README.TXT"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent unchecked
