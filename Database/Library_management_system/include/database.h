#ifndef DATABASE_H
#define DATABASE_H

#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Database connection configuration
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "Aart-123"
#define DB_NAME "library_management"
#define DB_PORT 3307

// Structure to hold database connection
typedef struct {
    MYSQL *conn;
    int connected;
} Database;

// Function declarations
Database* db_init(void);
void db_close(Database *db);
int db_connect(Database *db);
int db_execute_query(Database *db, const char *query);
MYSQL_RES* db_execute_select(Database *db, const char *query);
void db_free_result(MYSQL_RES *result);

// Error handling
void db_print_error(Database *db);

#endif // DATABASE_H 