#ifndef SQLITE_DB_H
#define SQLITE_DB_H

#include "structures.h"
#include <sqlite3.h>

// Utility and unique function declarations only. CRUD declarations are handled by library_crud.h

// Example: open/close database
int openDatabase(const char* filename, sqlite3** db);
void closeDatabase(sqlite3* db);

// Add any other unique utility functions here

#endif // SQLITE_DB_H 