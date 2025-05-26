@echo off
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