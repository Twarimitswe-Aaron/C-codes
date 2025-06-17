#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "sqlite_db.h"
#include "date_utils.h"

static sqlite3* db = NULL;

int sql_initDatabase() {
    int rc = sqlite3_open("library.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

void sql_closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}
// Only keep database open/close and utility functions here. 