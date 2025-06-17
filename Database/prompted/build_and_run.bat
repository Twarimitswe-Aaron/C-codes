 @echo off
echo Compiling project...
gcc -o bin/library_gui.exe library_gui.c library_crud.c sqlite_db.c -I./include -lsqlite3 -lcomctl32 -lgdi32 -lcomdlg32 -luxtheme
if %ERRORLEVEL% EQU 0 (
    echo Build successful! Running application...
    bin\library_gui.exe
) else (
    echo Build failed with error code %ERRORLEVEL%.
    pause
)
