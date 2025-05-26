#ifndef AUTHOR_H
#define AUTHOR_H

#include "database.h"
#include <time.h>

// Author structure
typedef struct {
    int author_id;
    char name[100];
    char bio[1000];
    time_t created_at;
    time_t updated_at;
} Author;

// Function declarations
Author* author_create(const char *name, const char *bio);
void author_free(Author *author);

// Database operations
int author_save(Database *db, Author *author);
int author_update(Database *db, Author *author);
int author_delete(Database *db, int author_id);
Author* author_find_by_id(Database *db, int author_id);
Author* author_find_by_name(Database *db, const char *name);

// List operations
typedef struct {
    Author **authors;
    int count;
    int capacity;
} AuthorList;

AuthorList* author_list_create(int initial_capacity);
void author_list_free(AuthorList *list);
int author_list_add(AuthorList *list, Author *author);
AuthorList* author_list_all(Database *db);

#endif // AUTHOR_H 