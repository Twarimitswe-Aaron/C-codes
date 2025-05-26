@echo off
echo Compiling Library Management System...

REM Compile console version
gcc main.c database.c -o library_system_console.exe

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo.
    echo To run the program:
    echo   - For console mode: library_system_console.exe
) else (
    echo Compilation failed!
)

pause 