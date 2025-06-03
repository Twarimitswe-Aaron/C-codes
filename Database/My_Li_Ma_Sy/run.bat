@echo off
setlocal EnableDelayedExpansion

echo Compiling...
gcc -Wall -Wextra -o li_ma_sy.exe main.c Database/database.c models/author.c -lmysqlclient -I "C:/msys64/mingw64/include"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Compilation successful! Running the program...
    echo ==========================================
    li_ma_sy.exe
    echo ==========================================
    echo Program finished.
) else (
    echo.
    echo *** COMPILATION FAILED ***
    echo Error code: %ERRORLEVEL%
    echo Please check the error messages above.
)

echo.
pause