@echo off
setlocal

REM Compile console version
if exist schema_generator.exe del schema_generator.exe
if exist schema_gui.exe del schema_gui.exe

echo Compiling schema_generator.c (console version)...
gcc -o schema_generator.exe schema_generator.c -lsqlite3
if %errorlevel% neq 0 (
    echo Failed to compile schema_generator.c
    goto END
)

echo Compiling schema_gui.c (GUI version)...
gcc -o schema_gui.exe schema_gui.c -lsqlite3 -lcomctl32 -lgdi32 -lcomdlg32 -mwindows
if %errorlevel% neq 0 (
    echo Failed to compile schema_gui.c
    goto END
)

echo.
echo Compilation successful!
echo.
echo Choose which version to run:
echo   1. Console version (schema_generator.exe)
echo   2. GUI version (schema_gui.exe)
set /p choice=Enter 1 or 2: 
if "%choice%"=="1" (
    schema_generator.exe schema.txt
) else if "%choice%"=="2" (
    start schema_gui.exe
) else (
    echo Invalid choice. Exiting.
)

:END
pause
endlocal 