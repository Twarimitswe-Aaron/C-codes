#include "../../include/author.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new author instance
 */
Author* author_create(const char *name, const char *bio) {
    Author *author = (Author*)malloc(sizeof(Author));
    if (author == NULL) {
        return NULL;
    }

    // Initialize fields
    author->author_id = 0;  // Will be set by database
    strncpy(author->name, name, sizeof(author->name) - 1);
    author->name[sizeof(author->name) - 1] = '\0';
    strncpy(author->bio, bio, sizeof(author->bio) - 1);
    author->bio[sizeof(author->bio) - 1] = '\0';
    author->created_at = time(NULL);
    author->updated_at = author->created_at;

    return author;
}

/**
 * Free author resources
 */
void author_free(Author *author) {
    if (author != NULL) {
        free(author);
    }
}

/**
 * Save a new author to the database
 */
int author_save(Database *db, Author *author) {
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO authors (name, bio) VALUES ('%s', '%s')",
        author->name, author->bio);

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
        author->author_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing author in the database
 */
int author_update(Database *db, Author *author) {
    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE authors SET name='%s', bio='%s' WHERE author_id=%d",
        author->name, author->bio, author->author_id);

    author->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete an author from the database
 */
int author_delete(Database *db, int author_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM authors WHERE author_id=%d", author_id);
    return db_execute_query(db, query);
}

/**
 * Find an author by ID
 */
Author* author_find_by_id(Database *db, int author_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM authors WHERE author_id=%d", author_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Author *author = (Author*)malloc(sizeof(Author));
    if (author == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to author structure
    author->author_id = atoi(row[0]);
    strncpy(author->name, row[1], sizeof(author->name) - 1);
    strncpy(author->bio, row[2], sizeof(author->bio) - 1);

    db_free_result(result);
    return author;
}

/**
 * Find an author by name
 */
Author* author_find_by_name(Database *db, const char *name) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM authors WHERE name='%s'", name);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Author *author = (Author*)malloc(sizeof(Author));
    if (author == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to author structure
    author->author_id = atoi(row[0]);
    strncpy(author->name, row[1], sizeof(author->name) - 1);
    strncpy(author->bio, row[2], sizeof(author->bio) - 1);

    db_free_result(result);
    return author;
}

/**
 * Create a new author list
 */
AuthorList* author_list_create(int initial_capacity) {
    AuthorList *list = (AuthorList*)malloc(sizeof(AuthorList));
    if (list == NULL) {
        return NULL;
    }

    list->authors = (Author**)malloc(sizeof(Author*) * initial_capacity);
    if (list->authors == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free author list resources
 */
void author_list_free(AuthorList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            author_free(list->authors[i]);
        }
        free(list->authors);
        free(list);
    }
}

/**
 * Add an author to the list
 */
int author_list_add(AuthorList *list, Author *author) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Author **new_authors = (Author**)realloc(list->authors, sizeof(Author*) * new_capacity);
        if (new_authors == NULL) {
            return 0;
        }
        list->authors = new_authors;
        list->capacity = new_capacity;
    }

    list->authors[list->count++] = author;
    return 1;
}

/**
 * Get all authors from the database
 */
AuthorList* author_list_all(Database *db) {
    const char *query = "SELECT * FROM authors";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    AuthorList *list = author_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Author *author = (Author*)malloc(sizeof(Author));
        if (author == NULL) {
            continue;
        }

        author->author_id = atoi(row[0]);
        strncpy(author->name, row[1], sizeof(author->name) - 1);
        strncpy(author->bio, row[2], sizeof(author->bio) - 1);

        author_list_add(list, author);
    }

    db_free_result(result);
    return list;
} 