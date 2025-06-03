@echo off
echo Compiling Library Management System...

:: Compile main.c
gcc -c main.c -o main.o
if errorlevel 1 (
    echo Compilation of main.c failed!
    pause
    exit /b 1
)

:: Compile console.c
gcc -c console.c -o console.o
if errorlevel 1 (
    echo Compilation of console.c failed!
    pause
    exit /b 1
)

:: Compile gui.c
gcc -c gui.c -o gui.o
if errorlevel 1 (
    echo Compilation of gui.c failed!
    pause
    exit /b 1
)

:: Compile database.c (optional future use)
gcc -c database.c -o database.o
if errorlevel 1 (
    echo Compilation of database.c failed!
    pause
    exit /b 1
)



:: Compile manager.c (newly added file)
gcc -c manager.c -o manager.o
if errorlevel 1 (
    echo Compilation of manager.c failed!
    pause
    exit /b 1
)

:: Link all object files
gcc main.o console.o gui.o database.o  manager.o -o library_system.exe -lgdi32 -luser32 -lkernel32 -lcomctl32
if errorlevel 1 (
    echo Linking failed!
    pause
    exit /b 1
)

echo.
echo Compilation and linking successful!
echo --------------------------------------
echo Running Library Management System...
echo --------------------------------------
echo.

library_system.exe
pause
