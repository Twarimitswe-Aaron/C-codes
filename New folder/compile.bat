@echo off
echo Compiling Library Management System...

if not exist "bin" mkdir bin

gcc -o bin/library_system.exe src/main.c src/database.c -I include

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running the program...
    bin\library_system.exe
) else (
    echo Compilation failed!
    pause
) 