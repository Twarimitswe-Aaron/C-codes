#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h> // Requires linking -lmysqlclient
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_HOST "localhost"
#define DB_USER "root"       // Consider moving to config file
#define DB_PASS "Aart-123"   // Avoid hardcoding in production
#define DB_NAME "li_ma_sy"
#define DB_PORT 3307         // Verify if this is correct (default: 3306)

typedef struct {
    MYSQL *conn;            // MySQL connection instance
    int connected;          // Connection status flag
} Database;

Database* db_init(void);               // Initialize database
void db_close(Database *db);           // Close connection
int db_connect(Database *db);          // Establish connection
int db_execute_query(Database *db, const char *query);  // Execute INSERT/UPDATE/DELETE
MYSQL_RES* db_execute_select(Database *db, const char *query);  // Execute SELECT
void db_free_result(MYSQL_RES *result); // Free result memory
void db_print_error(Database *db);      // Print MySQL errors

#endif