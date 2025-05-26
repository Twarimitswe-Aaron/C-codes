#include "../../include/publisher.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new publisher instance
 */
Publisher* publisher_create(const char *name, const char *address, const char *contact_info) {
    Publisher *publisher = (Publisher*)malloc(sizeof(Publisher));
    if (publisher == NULL) {
        return NULL;
    }

    // Initialize fields
    publisher->publisher_id = 0;  // Will be set by database
    strncpy(publisher->name, name, sizeof(publisher->name) - 1);
    publisher->name[sizeof(publisher->name) - 1] = '\0';
    strncpy(publisher->address, address, sizeof(publisher->address) - 1);
    publisher->address[sizeof(publisher->address) - 1] = '\0';
    strncpy(publisher->contact_info, contact_info, sizeof(publisher->contact_info) - 1);
    publisher->contact_info[sizeof(publisher->contact_info) - 1] = '\0';
    publisher->created_at = time(NULL);
    publisher->updated_at = publisher->created_at;

    return publisher;
}

/**
 * Free publisher resources
 */
void publisher_free(Publisher *publisher) {
    if (publisher != NULL) {
        free(publisher);
    }
}

/**
 * Save a new publisher to the database
 */
int publisher_save(Database *db, Publisher *publisher) {
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO publishers (name, address, contact_info) VALUES ('%s', '%s', '%s')",
        publisher->name, publisher->address, publisher->contact_info);

    if (!db_execute_query(db, query)) {
        return 0;
    }

    // Get the inserted ID
    MYSQL_RES *result = db_execute_select(db, "SELECT LAST_INSERT_ID()");
    if (result == NULL) {
        return 0;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL) {
        publisher->publisher_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing publisher in the database
 */
int publisher_update(Database *db, Publisher *publisher) {
    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE publishers SET name='%s', address='%s', contact_info='%s' WHERE publisher_id=%d",
        publisher->name, publisher->address, publisher->contact_info, publisher->publisher_id);

    publisher->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete a publisher from the database
 */
int publisher_delete(Database *db, int publisher_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM publishers WHERE publisher_id=%d", publisher_id);
    return db_execute_query(db, query);
}

/**
 * Find a publisher by ID
 */
Publisher* publisher_find_by_id(Database *db, int publisher_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM publishers WHERE publisher_id=%d", publisher_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Publisher *publisher = (Publisher*)malloc(sizeof(Publisher));
    if (publisher == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to publisher structure
    publisher->publisher_id = atoi(row[0]);
    strncpy(publisher->name, row[1], sizeof(publisher->name) - 1);
    strncpy(publisher->address, row[2], sizeof(publisher->address) - 1);
    strncpy(publisher->contact_info, row[3], sizeof(publisher->contact_info) - 1);

    db_free_result(result);
    return publisher;
}

/**
 * Find a publisher by name
 */
Publisher* publisher_find_by_name(Database *db, const char *name) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM publishers WHERE name='%s'", name);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Publisher *publisher = (Publisher*)malloc(sizeof(Publisher));
    if (publisher == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to publisher structure
    publisher->publisher_id = atoi(row[0]);
    strncpy(publisher->name, row[1], sizeof(publisher->name) - 1);
    strncpy(publisher->address, row[2], sizeof(publisher->address) - 1);
    strncpy(publisher->contact_info, row[3], sizeof(publisher->contact_info) - 1);

    db_free_result(result);
    return publisher;
}

/**
 * Create a new publisher list
 */
PublisherList* publisher_list_create(int initial_capacity) {
    PublisherList *list = (PublisherList*)malloc(sizeof(PublisherList));
    if (list == NULL) {
        return NULL;
    }

    list->publishers = (Publisher**)malloc(sizeof(Publisher*) * initial_capacity);
    if (list->publishers == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free publisher list resources
 */
void publisher_list_free(PublisherList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            publisher_free(list->publishers[i]);
        }
        free(list->publishers);
        free(list);
    }
}

/**
 * Add a publisher to the list
 */
int publisher_list_add(PublisherList *list, Publisher *publisher) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Publisher **new_publishers = (Publisher**)realloc(list->publishers, sizeof(Publisher*) * new_capacity);
        if (new_publishers == NULL) {
            return 0;
        }
        list->publishers = new_publishers;
        list->capacity = new_capacity;
    }

    list->publishers[list->count++] = publisher;
    return 1;
}

/**
 * Get all publishers from the database
 */
PublisherList* publisher_list_all(Database *db) {
    const char *query = "SELECT * FROM publishers";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    PublisherList *list = publisher_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Publisher *publisher = (Publisher*)malloc(sizeof(Publisher));
        if (publisher == NULL) {
            continue;
        }

        publisher->publisher_id = atoi(row[0]);
        strncpy(publisher->name, row[1], sizeof(publisher->name) - 1);
        strncpy(publisher->address, row[2], sizeof(publisher->address) - 1);
        strncpy(publisher->contact_info, row[3], sizeof(publisher->contact_info) - 1);

        publisher_list_add(list, publisher);
    }

    db_free_result(result);
    return list;
} 