@echo off
echo Building Library Management System with debug flags...

:: Create build directory if it doesn't exist
if not exist build mkdir build

:: Kill any existing process
taskkill /F /IM library_system_console.exe 2>nul

:: Remove old executable if it exists
if exist build\library_system_console.exe del build\library_system_console.exe

:: Compile with debug flags
gcc -g -Wall -Wextra -o build\library_system_console.exe main.c console_ui.c database.c

if %errorlevel% equ 0 (
    echo.
    echo Build successful! You can run the program with:
    echo build\library_system_console.exe
) else (
    echo.
    echo Build failed! Please check the errors above.
)

pause 