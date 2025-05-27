cmake_minimum_required(VERSION 3.10)
project(Library_management_system)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set PKG_CONFIG_PATH to help find GTK
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

# Find GTK3 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Find MariaDB
set(MARIADB_ROOT "C:/msys64/mingw64")
set(MARIADB_INCLUDE_DIR "${MARIADB_ROOT}/include/mysql")
set(MARIADB_LIB_DIR "${MARIADB_ROOT}/lib")

# Add GTK3 include directories
include_directories(
    ${GTK3_INCLUDE_DIRS}
    ${MARIADB_INCLUDE_DIR}
)

# Add compile definitions
add_definitions(${GTK3_CFLAGS_OTHER} -D_WIN32_WINNT=0x0601)

# Add source files
set(SOURCES
    src/main.c
    src/database/database.c
    src/models/book.c
    src/models/author.c
    src/models/publisher.c
    src/models/member.c
    src/models/staff.c
    src/models/borrowing.c
    src/models/fine.c
    src/gui/main_window.c
    src/gui/books_page.c
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against GTK3 and MariaDB libraries
target_link_libraries(${PROJECT_NAME} 
    ${GTK3_LIBRARIES}
    "${MARIADB_LIB_DIR}/libmariadb.dll.a"
)

# Add compile options
target_compile_options(${PROJECT_NAME} PRIVATE 
    ${GTK3_CFLAGS_OTHER}
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-unused-variable
)

# Copy GTK3 DLLs to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/msys64/mingw64/bin"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Copy MariaDB DLL to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${MARIADB_LIB_DIR}/libmariadb.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
) 