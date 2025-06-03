#ifndef AUTHOR_H
#define AUTHOR_H

#include <time.h>
#include "./database.h"


typedef struct {
    int author_id;
    char name[50];
    char email[50];
    time_t last_login;
    time_t created_at;
    time_t updated_at;

}Author;

Author* author_create(const char *name, const char *email);
void author_free(Author *author);

int author_save(Database *db, Author *author);
int author_update(Database *db, Author *author);
int author_delete(Database *db, int author_id);
Author* author_find_by_id(Database *db, int author_id);
Author* author_find_by_name(Database *db, const char *name);

typedef struct {
    Author **authors;
    int count;
    int capacity;
}AuthorList;


AuthorList* author_list_create(int intitial_capacity);
void author_list_free(AuthorList *list);
AuthorList* author_list_all(Database *db);

#endif


