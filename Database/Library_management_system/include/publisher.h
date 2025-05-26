#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "database.h"
#include <time.h>

// Publisher structure
typedef struct {
    int publisher_id;
    char name[100];
    char address[255];
    char contact_info[100];
    time_t created_at;
    time_t updated_at;
} Publisher;

// Function declarations
Publisher* publisher_create(const char *name, const char *address, const char *contact_info);
void publisher_free(Publisher *publisher);

// Database operations
int publisher_save(Database *db, Publisher *publisher);
int publisher_update(Database *db, Publisher *publisher);
int publisher_delete(Database *db, int publisher_id);
Publisher* publisher_find_by_id(Database *db, int publisher_id);
Publisher* publisher_find_by_name(Database *db, const char *name);

// List operations
typedef struct {
    Publisher **publishers;
    int count;
    int capacity;
} PublisherList;

PublisherList* publisher_list_create(int initial_capacity);
void publisher_list_free(PublisherList *list);
int publisher_list_add(PublisherList *list, Publisher *publisher);
PublisherList* publisher_list_all(Database *db);

#endif // PUBLISHER_H 