@echo off
echo Compiling Library Management System...

:: Get GTK/GLib flags using pkg-config
for /f "usebackq tokens=*" %%a in (`pkg-config --cflags gtk+-3.0`) do set "GTK_CFLAGS=%%a"
for /f "usebackq tokens=*" %%a in (`pkg-config --libs gtk+-3.0`) do set "GTK_LIBS=%%a"

:: Compile all source files in one command
gcc -o library_management_system.exe ^
    src/main.c ^
    src/database/database.c ^
    src/models/book.c ^
    src/models/author.c ^
    src/models/publisher.c ^
    src/models/member.c ^
    src/models/staff.c ^
    src/models/borrowing.c ^
    src/models/fine.c ^
    src/gui/main_window.c ^
    src/gui/books_page.c ^
    src/book.c ^
    -I./include ^
    -I./src/gui ^
    -I./src ^
    %GTK_CFLAGS% ^
    -Wall ^
    -L"C:/Program Files/MySQL/MySQL Server 8.0/lib" ^
    -lmysql ^
    %GTK_LIBS% ^
    -lgdi32 ^
    -lcomctl32 ^
    -luxtheme ^
    -luser32 ^
    -lkernel32

:: Check if compilation was successful
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running Library Management System...
    library_management_system.exe
) else (
    echo Compilation failed!
    pause
    exit /b 1
)

pause
