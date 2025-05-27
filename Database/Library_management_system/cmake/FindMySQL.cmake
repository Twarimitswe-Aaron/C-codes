# FindMySQL.cmake
# Finds the MySQL client library
#
# This will define the following variables:
#
#   MySQL_FOUND        - True if the system has MySQL
#   MySQL_INCLUDE_DIRS - MySQL include directory
#   MySQL_LIBRARIES    - MySQL libraries

include(FindPackageHandleStandardArgs)

# Find MySQL include directory
find_path(MySQL_INCLUDE_DIRS
    NAMES mysql.h
    PATHS
        C:/msys64/mingw64/include/mysql
        C:/msys64/mingw64/include
        /usr/include/mysql
        /usr/local/include/mysql
)

# Find MySQL client library
find_library(MySQL_LIBRARIES
    NAMES mysqlclient libmysqlclient
    PATHS
        C:/msys64/mingw64/lib
        /usr/lib
        /usr/local/lib
)

find_package_handle_standard_args(MySQL
    REQUIRED_VARS MySQL_LIBRARIES MySQL_INCLUDE_DIRS
)

mark_as_advanced(MySQL_INCLUDE_DIRS MySQL_LIBRARIES) 