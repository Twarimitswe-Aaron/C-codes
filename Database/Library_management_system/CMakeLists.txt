cmake_minimum_required(VERSION 3.10)
project(Library_Management_System C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Find MySQL
set(MySQL_DIR "C:/Program Files/MySQL/MySQL Server 8.0/lib")
find_library(MYSQL_LIBRARY NAMES mysql mysqlclient PATHS ${MySQL_DIR} REQUIRED)
find_path(MYSQL_INCLUDE_DIR mysql.h PATHS "C:/Program Files/MySQL/MySQL Server 8.0/include" REQUIRED)

# Include directories
include_directories(include)
include_directories(${MYSQL_INCLUDE_DIR})

# Add source files
set(SOURCES
    src/main.c
    src/models/book.c
    src/models/author.c
    src/models/publisher.c
    src/models/member.c
    src/models/staff.c
    src/models/borrowing.c
    src/models/fine.c
    src/database/database.c
)

# Create executable
add_executable(library_system ${SOURCES})

# Link MySQL
target_link_libraries(library_system ${MYSQL_LIBRARY}) 