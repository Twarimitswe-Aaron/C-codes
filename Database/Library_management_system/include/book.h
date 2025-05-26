#ifndef BOOK_H
#define BOOK_H

#include "database.h"
#include <time.h>

// Book structure
typedef struct {
    int book_id;
    char title[255];
    int author_id;
    int publisher_id;
    char isbn[13];
    char genre[50];
    int year_published;
    int copies_available;
    char shelf_location[50];
    time_t created_at;
    time_t updated_at;
} Book;

// Function declarations
Book* book_create(const char *title, int author_id, int publisher_id, 
                 const char *isbn, const char *genre, int year_published,
                 int copies_available, const char *shelf_location);
void book_free(Book *book);

// Database operations
int book_save(Database *db, Book *book);
int book_update(Database *db, Book *book);
int book_delete(Database *db, int book_id);
Book* book_find_by_id(Database *db, int book_id);
Book* book_find_by_title(Database *db, const char *title);
Book* book_find_by_isbn(Database *db, const char *isbn);

// List operations
typedef struct {
    Book **books;
    int count;
    int capacity;
} BookList;

BookList* book_list_create(int initial_capacity);
void book_list_free(BookList *list);
int book_list_add(BookList *list, Book *book);
BookList* book_list_all(Database *db);
BookList* book_list_by_author(Database *db, int author_id);
BookList* book_list_by_publisher(Database *db, int publisher_id);
BookList* book_list_by_genre(Database *db, const char *genre);

#endif // BOOK_H 