#ifndef BORROWING_H
#define BORROWING_H

#include "database.h"
#include <time.h>

// Borrowing status enum
typedef enum {
    BORROWED = 0,
    RETURNED = 1,
    OVERDUE = 2
} BorrowingStatus;

// Borrowing structure
typedef struct {
    int borrowing_id;
    int book_id;
    int member_id;
    int staff_id;
    time_t borrow_date;
    time_t due_date;
    time_t return_date;
    BorrowingStatus status;
    time_t created_at;
    time_t updated_at;
} Borrowing;

// Borrowing list structure
typedef struct {
    Borrowing **borrowings;
    int count;
    int capacity;
} BorrowingList;

// Create a new borrowing instance
Borrowing* borrowing_create(int book_id, int member_id, int staff_id, time_t borrow_date, time_t due_date);

// Free borrowing resources
void borrowing_free(Borrowing *borrowing);

// Save a new borrowing to the database
int borrowing_save(Database *db, Borrowing *borrowing);

// Update an existing borrowing in the database
int borrowing_update(Database *db, Borrowing *borrowing);

// Delete a borrowing from the database
int borrowing_delete(Database *db, int borrowing_id);

// Find a borrowing by ID
Borrowing* borrowing_find_by_id(Database *db, int borrowing_id);

// Find borrowings by book ID
BorrowingList* borrowing_list_by_book(Database *db, int book_id);

// Find borrowings by member ID
BorrowingList* borrowing_list_by_member(Database *db, int member_id);

// Find borrowings by status
BorrowingList* borrowing_list_by_status(Database *db, BorrowingStatus status);

// Create a new borrowing list
BorrowingList* borrowing_list_create(int initial_capacity);

// Free borrowing list resources
void borrowing_list_free(BorrowingList *list);

// Add a borrowing to the list
int borrowing_list_add(BorrowingList *list, Borrowing *borrowing);

// Get all borrowings from the database
BorrowingList* borrowing_list_all(Database *db);

#endif // BORROWING_H 