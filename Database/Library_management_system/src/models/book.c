#include "../../include/book.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new book instance
 */
Book* book_create(const char *title, int author_id, int publisher_id, 
                 const char *isbn, const char *genre, int year_published,
                 int copies_available, const char *shelf_location) {
    Book *book = (Book*)malloc(sizeof(Book));
    if (book == NULL) {
        return NULL;
    }

    // Initialize fields
    book->book_id = 0;  // Will be set by database
    strncpy(book->title, title, sizeof(book->title) - 1);
    book->title[sizeof(book->title) - 1] = '\0';
    book->author_id = author_id;
    book->publisher_id = publisher_id;
    strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
    book->isbn[sizeof(book->isbn) - 1] = '\0';
    strncpy(book->genre, genre, sizeof(book->genre) - 1);
    book->genre[sizeof(book->genre) - 1] = '\0';
    book->year_published = year_published;
    book->copies_available = copies_available;
    strncpy(book->shelf_location, shelf_location, sizeof(book->shelf_location) - 1);
    book->shelf_location[sizeof(book->shelf_location) - 1] = '\0';
    book->created_at = time(NULL);
    book->updated_at = book->created_at;

    return book;
}

/**
 * Free book resources
 */
void book_free(Book *book) {
    if (book != NULL) {
        free(book);
    }
}

/**
 * Save a new book to the database
 */
int book_save(Database *db, Book *book) {
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO books (title, author_id, publisher_id, isbn, genre, "
        "year_published, copies_available, shelf_location) "
        "VALUES ('%s', %d, %d, '%s', '%s', %d, %d, '%s')",
        book->title, book->author_id, book->publisher_id, book->isbn,
        book->genre, book->year_published, book->copies_available,
        book->shelf_location);

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
        book->book_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing book in the database
 */
int book_update(Database *db, Book *book) {
    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE books SET title='%s', author_id=%d, publisher_id=%d, "
        "isbn='%s', genre='%s', year_published=%d, copies_available=%d, "
        "shelf_location='%s' WHERE book_id=%d",
        book->title, book->author_id, book->publisher_id, book->isbn,
        book->genre, book->year_published, book->copies_available,
        book->shelf_location, book->book_id);

    book->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete a book from the database
 */
int book_delete(Database *db, int book_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM books WHERE book_id=%d", book_id);
    return db_execute_query(db, query);
}

/**
 * Find a book by ID
 */
Book* book_find_by_id(Database *db, int book_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM books WHERE book_id=%d", book_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Book *book = (Book*)malloc(sizeof(Book));
    if (book == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to book structure
    book->book_id = atoi(row[0]);
    strncpy(book->title, row[1], sizeof(book->title) - 1);
    book->author_id = atoi(row[2]);
    book->publisher_id = atoi(row[3]);
    strncpy(book->isbn, row[4], sizeof(book->isbn) - 1);
    strncpy(book->genre, row[5], sizeof(book->genre) - 1);
    book->year_published = atoi(row[6]);
    book->copies_available = atoi(row[7]);
    strncpy(book->shelf_location, row[8], sizeof(book->shelf_location) - 1);

    db_free_result(result);
    return book;
}

/**
 * Create a new book list
 */
BookList* book_list_create(int initial_capacity) {
    BookList *list = (BookList*)malloc(sizeof(BookList));
    if (list == NULL) {
        return NULL;
    }

    list->books = (Book**)malloc(sizeof(Book*) * initial_capacity);
    if (list->books == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free book list resources
 */
void book_list_free(BookList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            book_free(list->books[i]);
        }
        free(list->books);
        free(list);
    }
}

/**
 * Add a book to the list
 */
int book_list_add(BookList *list, Book *book) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Book **new_books = (Book**)realloc(list->books, sizeof(Book*) * new_capacity);
        if (new_books == NULL) {
            return 0;
        }
        list->books = new_books;
        list->capacity = new_capacity;
    }

    list->books[list->count++] = book;
    return 1;
}

/**
 * Get all books from the database
 */
BookList* book_list_all(Database *db) {
    const char *query = "SELECT * FROM books";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    BookList *list = book_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Book *book = (Book*)malloc(sizeof(Book));
        if (book == NULL) {
            continue;
        }

        book->book_id = atoi(row[0]);
        strncpy(book->title, row[1], sizeof(book->title) - 1);
        book->author_id = atoi(row[2]);
        book->publisher_id = atoi(row[3]);
        strncpy(book->isbn, row[4], sizeof(book->isbn) - 1);
        strncpy(book->genre, row[5], sizeof(book->genre) - 1);
        book->year_published = atoi(row[6]);
        book->copies_available = atoi(row[7]);
        strncpy(book->shelf_location, row[8], sizeof(book->shelf_location) - 1);

        book_list_add(list, book);
    }

    db_free_result(result);
    return list;
} 