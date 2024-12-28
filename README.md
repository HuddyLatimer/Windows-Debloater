# Windows Debloat Utility

A lightweight C++ utility designed to remove unnecessary Windows applications and services, improving system performance and reducing bloatware. This tool safely removes pre-installed Windows applications while preserving system functionality.

## Features

- Removes pre-installed Windows applications (Edge, Office, Mail, Calendar, etc.)
- Forcefully uninstalls OneDrive and prevents reinstallation
- Disables Xbox-related services and components
- Cleans up associated folders and temporary files
- Creates a system restore point before making changes
- User-friendly console interface with color-coded status messages
- Error handling for protected system components

## Requirements

- Windows 10/11
- Administrator privileges
- Visual C++ Redistributable
- C++ compiler (for building from source)

## Usage

1. Download the compiled executable or build from source
2. Run the program as Administrator
3. Press any key to begin the debloating process
4. Wait for the process to complete
5. Restart your system when prompted

## Building from Source

```bash
# Using Visual Studio
cl windows_debloat.cpp

# Using MinGW
g++ windows_debloat.cpp -o windows_debloat.exe
```

## ⚠️ Important Notes

- Always create a backup before running any system modification tools
- Some Windows components may reinstall during major system updates
- Certain system apps cannot be completely removed due to Windows protection
- A system restart is recommended after running the utility

## Safe Removal List

The utility safely removes or disables:
- Microsoft Edge
- Microsoft Office (Store version)
- Mail and Calendar
- Your Phone
- OneDrive
- Weather
- Maps
- Groove Music
- Movies & TV
- People
- Xbox related apps
- Feedback Hub
- 3D Viewer

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License

MIT License - feel free to modify and use this code in your projects.

## Disclaimer

This tool is provided as-is without any warranty. Use at your own risk. Always backup your system before making significant changes.
