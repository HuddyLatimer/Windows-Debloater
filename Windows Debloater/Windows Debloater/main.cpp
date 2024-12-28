#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <vector>

using namespace std;

void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

bool executePowerShellCommand(const string& command) {
    string fullCommand = "powershell.exe -Command \"" + command + "\"";
    int result = system(fullCommand.c_str());
    return result == 0;
}

void removeXboxComponents() {
    cout << "\nRemoving Xbox components..." << endl;

    vector<string> commands = {

        "Stop-Service XboxNetApiSvc -Force; Set-Service XboxNetApiSvc -StartupType Disabled",
        "Stop-Service XblAuthManager -Force; Set-Service XblAuthManager -StartupType Disabled",
        "Stop-Service XblGameSave -Force; Set-Service XblGameSave -StartupType Disabled",
        "Stop-Service XboxGipSvc -Force; Set-Service XboxGipSvc -StartupType Disabled",

        "Get-AppxPackage Microsoft.XboxApp | Remove-AppxPackage -ErrorAction SilentlyContinue",
        "Get-AppxPackage Microsoft.XboxIdentityProvider | Remove-AppxPackage -ErrorAction SilentlyContinue",
        "Get-AppxPackage Microsoft.XboxSpeechToTextOverlay | Remove-AppxPackage -ErrorAction SilentlyContinue",
        "Get-AppxPackage Microsoft.XboxGamingOverlay | Remove-AppxPackage -ErrorAction SilentlyContinue",

       
        "Get-ScheduledTask -TaskPath \\Microsoft\\XblGameSave\\ | Disable-ScheduledTask",


        "Disable-WindowsOptionalFeature -Online -FeatureName XboxGamingOverlay -NoRestart -ErrorAction SilentlyContinue",
        "Disable-WindowsOptionalFeature -Online -FeatureName XboxGamingOverlayPresenceWriter -NoRestart -ErrorAction SilentlyContinue"
    };

    for (const auto& command : commands) {
        executePowerShellCommand(command);
    }
}

bool removeAppPackage(const string& packageName, bool isSystemApp) {
    cout << "Removing " << packageName << "..." << endl;

    string command;
    if (isSystemApp) {
        command = "Get-AppxProvisionedPackage -Online | Where-Object {$_.PackageName -like '*" +
            packageName + "*'} | Remove-AppxProvisionedPackage -Online -ErrorAction SilentlyContinue; " +
            "Get-AppxPackage *" + packageName + "* | Remove-AppxPackage -ErrorAction SilentlyContinue";
    }
    else {
        command = "Get-AppxPackage *" + packageName + "* | Remove-AppxPackage -ErrorAction SilentlyContinue";
    }

    return executePowerShellCommand(command);
}

void forceRemoveOneDrive() {
    cout << "\nForcefully removing OneDrive..." << endl;

    vector<string> commands = {
    
        "taskkill /f /im OneDrive.exe",
        "taskkill /f /im FileCoAuth.exe",

  
        "reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\OneDrive\" /v \"DisableFileSyncNGSC\" /t REG_DWORD /d 1 /f",
        "reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\OneDrive\" /v \"DisableFileSync\" /t REG_DWORD /d 1 /f",

  
        "if (Test-Path \"$env:SystemRoot\\System32\\OneDriveSetup.exe\") { Start-Process \"$env:SystemRoot\\System32\\OneDriveSetup.exe\" -ArgumentList \"/uninstall\" -Wait }",
        "if (Test-Path \"$env:SystemRoot\\SysWOW64\\OneDriveSetup.exe\") { Start-Process \"$env:SystemRoot\\SysWOW64\\OneDriveSetup.exe\" -ArgumentList \"/uninstall\" -Wait }",


        "Remove-Item -Path \"$env:UserProfile\\OneDrive\" -Recurse -Force -ErrorAction SilentlyContinue",
        "Remove-Item -Path \"$env:LOCALAPPDATA\\Microsoft\\OneDrive\" -Recurse -Force -ErrorAction SilentlyContinue",
        "Remove-Item -Path \"$env:PROGRAMDATA\\Microsoft OneDrive\" -Recurse -Force -ErrorAction SilentlyContinue",
        "Remove-Item -Path \"$env:SYSTEMDRIVE\\OneDriveTemp\" -Recurse -Force -ErrorAction SilentlyContinue",

       
        "reg delete \"HKCR\\CLSID\\{018D5C66-4533-4307-9B53-224DE2ED1FE6}\" /f",
        "reg delete \"HKCR\\Wow6432Node\\CLSID\\{018D5C66-4533-4307-9B53-224DE2ED1FE6}\" /f",

 
        "reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\NewStartPanel\" /v \"RemoveOneDriveFromExplorer\" /t REG_DWORD /d 1 /f"
    };

    for (const auto& command : commands) {
        executePowerShellCommand(command);
    }
}

int main() {
    SetConsoleTitleA("Windows Debloat Utility");

    setTextColor(10);
    cout << "Press Any Key to Debloat Windows..." << endl;
    setTextColor(7);
    _getch();

    cout << "\nStarting Windows Debloat Process...\n" << endl;

    cout << "Creating system restore point..." << endl;
    executePowerShellCommand("Checkpoint-Computer -Description \"Before Windows Debloat\" -RestorePointType MODIFY_SETTINGS");

    struct AppInfo {
        string name;
        string displayName;
        bool isSystemApp;
    };

    vector<AppInfo> apps = {
        {"Microsoft.Edge", "Microsoft Edge", true},
        {"Microsoft.Office", "Microsoft Office", false},
        {"microsoft.windowscommunicationsapps", "Mail and Calendar", false},
        {"Microsoft.YourPhone", "Your Phone", false},
        {"Microsoft.OneDrive", "OneDrive", false},
        {"Microsoft.BingWeather", "Weather", false},
        {"Microsoft.WindowsMaps", "Maps", false},
        {"Microsoft.ZuneMusic", "Groove Music", false},
        {"Microsoft.ZuneVideo", "Movies & TV", false},
        {"Microsoft.People", "People", false},
        {"Microsoft.MicrosoftSolitaireCollection", "Solitaire Collection", false},
        {"Microsoft.WindowsFeedbackHub", "Feedback Hub", false},
        {"Microsoft.Microsoft3DViewer", "3D Viewer", false}
    };


    for (const auto& app : apps) {
        removeAppPackage(app.name, app.isSystemApp);
    }


    removeXboxComponents();


    forceRemoveOneDrive();

    cout << "\nCleaning up remaining folders..." << endl;
    vector<string> foldersToDelete = {
        "$env:LOCALAPPDATA\\Microsoft\\Edge",
        "$env:LOCALAPPDATA\\Microsoft\\Office",
        "$env:PROGRAMDATA\\Microsoft\\Windows\\Start Menu\\Programs\\OneDrive"
    };

    for (const string& folder : foldersToDelete) {
        string command = "Remove-Item -Path \"" + folder + "\" -Recurse -Force -ErrorAction SilentlyContinue";
        executePowerShellCommand(command);
    }


    cout << "\nCleaning temporary files..." << endl;
    executePowerShellCommand("Remove-Item -Path \"$env:TEMP\\*\" -Recurse -Force -ErrorAction SilentlyContinue");

    setTextColor(10);
    cout << "\nDebloat process completed successfully!" << endl;
    cout << "Your system has been cleaned of unnecessary Windows apps and services." << endl;
    cout << "NOTE: Some system components may require a restart to fully remove." << endl;
    cout << "\nPress any key to exit..." << endl;
    setTextColor(7);

    _getch();
    return 0;
}