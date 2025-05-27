@echo off
setlocal EnableDelayedExpansion

REM Check if pkg-config is available
where pkg-config >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: pkg-config is not installed. Please install it first.
    exit /b 1
)

REM Get GTK flags
for /f "usebackq tokens=*" %%a in (`pkg-config --cflags gtk+-3.0`) do set "GTK_CFLAGS=%%a"
for /f "usebackq tokens=*" %%a in (`pkg-config --libs gtk+-3.0`) do set "GTK_LIBS=%%a"

if "%1"=="gui" goto gui

REM --- Console build (default) ---
gcc -Wall -Wextra -I./include ^
    src/main.c ^
    src/models/book.c ^
    src/models/author.c ^
    src/models/publisher.c ^
    src/models/member.c ^
    src/models/staff.c ^
    src/models/borrowing.c ^
    src/models/fine.c ^
    src/database/database.c ^
    -o library_system.exe ^
    -L"C:/Program Files/MySQL/MySQL Server 8.0/lib" ^
    -lmysql
goto end

:gui
REM --- GUI build ---
echo GTK CFLAGS: !GTK_CFLAGS!
echo GTK LIBS  : !GTK_LIBS!

gcc -Wall -Wextra -I./include !GTK_CFLAGS! ^
    src/main.c ^
    src/gui/main_window.c ^
    src/gui/books_page.c ^
    src/models/book.c ^
    src/models/author.c ^
    src/models/publisher.c ^
    src/models/member.c ^
    src/models/staff.c ^
    src/models/borrowing.c ^
    src/models/fine.c ^
    src/database/database.c ^
    -o library_system.exe ^
    -L"C:/Program Files/MySQL/MySQL Server 8.0/lib" ^
    -lmysql !GTK_LIBS!

echo Build complete: library_system.exe
goto end

:end
endlocal
