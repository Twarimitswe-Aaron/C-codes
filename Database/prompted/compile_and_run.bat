@echo off
echo Compiling Library Management System...

:: Compile each source file to an object file
gcc -c main.c -o main.o
if errorlevel 1 (
    echo Compilation of main.c failed!
    pause
    exit /b 1
)

:: Skip console.c if you're not using it
gcc -c console.c -o console.o
if errorlevel 1 (
    echo Compilation of console.c failed!
    pause
    exit /b 1
)

gcc -c gui.c -o gui.o
if errorlevel 1 (
    echo Compilation of gui.c failed!
    pause
    exit /b 1
)

gcc -c database.c -o database.o
if errorlevel 1 (
    echo Compilation of database.c failed!
    pause
    exit /b 1
)

gcc -c book_manager.c -o book_manager.o
if errorlevel 1 (
    echo Compilation of book_manager.c failed!
    pause
    exit /b 1
)

:: Link the object files into an executable
gcc main.c console.c gui.c database.c book_manager.c -o library_system.exe -lgdi32 -luser32 -lkernel32 -lcomctl32
if errorlevel 1 (
    echo Linking failed!
    pause
    exit /b 1
)

echo Compilation and linking successful!
echo Running Library Management System...
library_system.exe
pause