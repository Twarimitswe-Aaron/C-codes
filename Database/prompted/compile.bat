@echo off
echo Compiling schema_generator.c...
gcc -o schema_generator.exe schema_generator.c -lsqlite3
if %errorlevel% equ 0 (
    echo Compilation successful!
    echo You can now run the program using: schema_generator.exe schema.txt [database.db]
) else (
    echo Compilation failed!
)
pause 