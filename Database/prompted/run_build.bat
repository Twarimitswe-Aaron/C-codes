@echo off
echo Building Library Management System...

:: Create database directory if it doesn't exist
if not exist "database" mkdir database

:: Clean up old object files
del *.o 2>nul

:: Compile source files with proper flags
gcc -c gui.c -o gui.o -Wall
gcc -c console.c -o console.o -Wall
gcc -c database.c -o database.o -Wall
gcc -c main.c -o main.o -Wall
gcc -c date_utils.c -o date_utils.o -Wall

:: Check if compilation was successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

:: Link object files with required libraries
gcc main.o gui.o console.o database.o date_utils.o -o library_system.exe -lgdi32 -lcomctl32 -luxtheme -luser32 -lkernel32

:: Check if linking was successful
if %errorlevel% equ 0 (
    echo Build successful!
    echo Running Library Management System...
    library_system.exe
) else (
    echo Linking failed!
    pause
    exit /b 1
)

:: Clean up object files
del *.o 