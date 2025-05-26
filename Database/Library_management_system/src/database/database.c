#include "../../include/database.h"

/**
 * Initialize a new database connection structure
 * @return Pointer to initialized Database structure
 */
Database* db_init(void) {
    printf("Initializing database connection...\n");
    Database *db = (Database*)malloc(sizeof(Database));
    if (db == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for database connection\n");
        return NULL;
    }
    
    printf("Calling mysql_init()...\n");
    db->conn = mysql_init(NULL);
    if (db->conn == NULL) {
        fprintf(stderr, "Error: Failed to initialize MySQL connection: %s\n", mysql_error(NULL));
        free(db);
        return NULL;
    }
    printf("mysql_init() successful\n");
    
    // Set connection options
    printf("Setting connection options...\n");
    if (mysql_options(db->conn, MYSQL_OPT_CONNECT_TIMEOUT, "10") != 0) {
        fprintf(stderr, "Error: Failed to set connection timeout: %s\n", mysql_error(db->conn));
        mysql_close(db->conn);
        free(db);
        return NULL;
    }
    printf("Connection options set successfully\n");
    
    db->connected = 0;
    return db;
}

/**
 * Connect to the MySQL database
 * @param db Database structure pointer
 * @return 1 on success, 0 on failure
 */
int db_connect(Database *db) {
    printf("\nAttempting to connect to MySQL...\n");
    printf("Connection details:\n");
    printf("  Host: %s\n", DB_HOST);
    printf("  User: %s\n", DB_USER);
    printf("  Database: %s\n", DB_NAME);
    printf("  Port: %d\n", DB_PORT);
    
    if (!mysql_real_connect(db->conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, NULL, 0)) {
        fprintf(stderr, "Error connecting to MySQL: %s\n", mysql_error(db->conn));
        fprintf(stderr, "Error code: %d\n", mysql_errno(db->conn));
        return 0;
    }
    
    printf("Successfully connected to MySQL!\n");
    db->connected = 1;
    return 1;
}

/**
 * Close the database connection and free resources
 * @param db Database structure pointer
 */
void db_close(Database *db) {
    if (db != NULL) {
        if (db->connected) {
            mysql_close(db->conn);
        }
        free(db);
    }
}

/**
 * Execute a non-select SQL query
 * @param db Database structure pointer
 * @param query SQL query string
 * @return 1 on success, 0 on failure
 */
int db_execute_query(Database *db, const char *query) {
    if (!db->connected) {
        fprintf(stderr, "Error: Not connected to database\n");
        return 0;
    }
    
    if (mysql_query(db->conn, query)) {
        db_print_error(db);
        return 0;
    }
    return 1;
}

/**
 * Execute a SELECT SQL query
 * @param db Database structure pointer
 * @param query SQL query string
 * @return MYSQL_RES pointer on success, NULL on failure
 */
MYSQL_RES* db_execute_select(Database *db, const char *query) {
    if (!db->connected) {
        fprintf(stderr, "Error: Not connected to database\n");
        return NULL;
    }
    
    if (mysql_query(db->conn, query)) {
        db_print_error(db);
        return NULL;
    }
    
    return mysql_store_result(db->conn);
}

/**
 * Free the result set
 * @param result MYSQL_RES pointer
 */
void db_free_result(MYSQL_RES *result) {
    if (result != NULL) {
        mysql_free_result(result);
    }
}

/**
 * Print the last MySQL error
 * @param db Database structure pointer
 */
void db_print_error(Database *db) {
    fprintf(stderr, "MySQL Error: %s\n", mysql_error(db->conn));
}

void db_disconnect(Database *db) {
    db_close(db);
} 