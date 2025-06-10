@echo off
echo Compiling schema_gui.c...
gcc -o schema_gui.exe schema_gui.c -lsqlite3 -lcomctl32 -lgdi32 -lcomdlg32 -mwindows
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo Running schema_gui.exe...
    start schema_gui.exe
) else (
    echo Compilation failed!
    echo Error: Could not compile schema_gui.c
    echo Please check if you have all required libraries installed:
    echo - sqlite3
    echo - comctl32
    echo - gdi32
    echo - comdlg32
)
pause 