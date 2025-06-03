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
int author_save(Database *db, Author* author) {
    char escaped_name[200];
    char escaped_bio[2000];
    
    // Escape the strings
    mysql_real_escape_string(db->conn, escaped_name, author->name, strlen(author->name));
    mysql_real_escape_string(db->conn, escaped_bio, author->bio, strlen(author->bio));
    
    // Use prepared statement to avoid buffer overflow
    MYSQL_STMT *stmt = mysql_stmt_init(db->conn);
    if (!stmt) {
        return 0;
    }

    const char *query_str = "INSERT INTO authors (name, bio) VALUES (?, ?)";
    if (mysql_stmt_prepare(stmt, query_str, strlen(query_str)) != 0) {
        mysql_stmt_close(stmt);
        return 0;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    // Bind name
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = escaped_name;
    bind[0].buffer_length = strlen(escaped_name);
    bind[0].is_null = 0;

    // Bind bio
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = escaped_bio;
    bind[1].buffer_length = strlen(escaped_bio);
    bind[1].is_null = 0;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        mysql_stmt_close(stmt);
        return 0;
    }

    int result = (mysql_stmt_execute(stmt) == 0);
    mysql_stmt_close(stmt);
    return result;
}

/**
 * Update an existing author in the database
 */
int author_update(Database *db, Author *author) {
    char escaped_name[200];
    char escaped_bio[2000];
    
    // Escape the strings
    mysql_real_escape_string(db->conn, escaped_name, author->name, strlen(author->name));
    mysql_real_escape_string(db->conn, escaped_bio, author->bio, strlen(author->bio));
    
    // Use prepared statement to avoid buffer overflow
    MYSQL_STMT *stmt = mysql_stmt_init(db->conn);
    if (!stmt) {
        return 0;
    }

    const char *query_str = "UPDATE authors SET name=?, bio=? WHERE author_id=?";
    if (mysql_stmt_prepare(stmt, query_str, strlen(query_str)) != 0) {
        mysql_stmt_close(stmt);
        return 0;
    }

    MYSQL_BIND bind[3];
    memset(bind, 0, sizeof(bind));

    // Bind name
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = escaped_name;
    bind[0].buffer_length = strlen(escaped_name);
    bind[0].is_null = 0;

    // Bind bio
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = escaped_bio;
    bind[1].buffer_length = strlen(escaped_bio);
    bind[1].is_null = 0;

    // Bind author_id
    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &author->author_id;
    bind[2].is_null = 0;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        mysql_stmt_close(stmt);
        return 0;
    }

    author->updated_at = time(NULL);
    int result = (mysql_stmt_execute(stmt) == 0);
    mysql_stmt_close(stmt);
    return result;
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