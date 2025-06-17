@echo off
setlocal

REM Set compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -I. -I./include
set LDFLAGS=-lsqlite3 -lcomctl32 -lgdi32 -lcomdlg32 -luxtheme

REM Create output directory if it doesn't exist
if not exist "bin" mkdir bin

REM Compile the program
%CC% %CFLAGS% -c library_gui.c -o bin/library_gui.o
%CC% %CFLAGS% -c database.c -o bin/database.o
%CC% %CFLAGS% -c date_utils.c -o bin/date_utils.o
%CC% %CFLAGS% -c sqlite_db.c -o bin/sqlite_db.o

REM Link the program
%CC% bin/library_gui.o bin/database.o bin/date_utils.o bin/sqlite_db.o -o bin/library_gui.exe %LDFLAGS%

REM Check if compilation was successful
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Executable created at bin/library_gui.exe
    echo Running the program...
    start bin/library_gui.exe
) else (
    echo Compilation failed!
)

endlocal 