#include "../../include/borrowing.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new borrowing instance
 */
Borrowing* borrowing_create(int book_id, int member_id, int staff_id, time_t borrow_date, time_t due_date) {
    Borrowing *borrowing = (Borrowing*)malloc(sizeof(Borrowing));
    if (borrowing == NULL) {
        return NULL;
    }

    // Initialize fields
    borrowing->borrowing_id = 0;  // Will be set by database
    borrowing->book_id = book_id;
    borrowing->member_id = member_id;
    borrowing->staff_id = staff_id;
    borrowing->borrow_date = borrow_date;
    borrowing->due_date = due_date;
    borrowing->return_date = 0;  // Not returned yet
    borrowing->status = BORROWED;
    borrowing->created_at = time(NULL);
    borrowing->updated_at = borrowing->created_at;

    return borrowing;
}

/**
 * Free borrowing resources
 */
void borrowing_free(Borrowing *borrowing) {
    if (borrowing != NULL) {
        free(borrowing);
    }
}

/**
 * Save a new borrowing to the database
 */
int borrowing_save(Database *db, Borrowing *borrowing) {
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO borrowings (book_id, member_id, staff_id, borrow_date, due_date, status) "
        "VALUES (%d, %d, %d, FROM_UNIXTIME(%lld), FROM_UNIXTIME(%lld), %d)",
        borrowing->book_id, borrowing->member_id, borrowing->staff_id,
        borrowing->borrow_date, borrowing->due_date, borrowing->status);

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
        borrowing->borrowing_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing borrowing in the database
 */
int borrowing_update(Database *db, Borrowing *borrowing) {
    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE borrowings SET book_id=%d, member_id=%d, staff_id=%d, "
        "borrow_date=FROM_UNIXTIME(%lld), due_date=FROM_UNIXTIME(%lld), "
        "return_date=FROM_UNIXTIME(%lld), status=%d WHERE borrowing_id=%d",
        borrowing->book_id, borrowing->member_id, borrowing->staff_id,
        borrowing->borrow_date, borrowing->due_date, borrowing->return_date,
        borrowing->status, borrowing->borrowing_id);

    borrowing->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete a borrowing from the database
 */
int borrowing_delete(Database *db, int borrowing_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM borrowings WHERE borrowing_id=%d", borrowing_id);
    return db_execute_query(db, query);
}

/**
 * Find a borrowing by ID
 */
Borrowing* borrowing_find_by_id(Database *db, int borrowing_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM borrowings WHERE borrowing_id=%d", borrowing_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Borrowing *borrowing = (Borrowing*)malloc(sizeof(Borrowing));
    if (borrowing == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to borrowing structure
    borrowing->borrowing_id = atoi(row[0]);
    borrowing->book_id = atoi(row[1]);
    borrowing->member_id = atoi(row[2]);
    borrowing->staff_id = atoi(row[3]);
    borrowing->borrow_date = atoi(row[4]);
    borrowing->due_date = atoi(row[5]);
    borrowing->return_date = atoi(row[6]);
    borrowing->status = atoi(row[7]);

    db_free_result(result);
    return borrowing;
}

/**
 * Find borrowings by book ID
 */
BorrowingList* borrowing_list_by_book(Database *db, int book_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM borrowings WHERE book_id=%d", book_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    BorrowingList *list = borrowing_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Borrowing *borrowing = (Borrowing*)malloc(sizeof(Borrowing));
        if (borrowing == NULL) {
            continue;
        }

        borrowing->borrowing_id = atoi(row[0]);
        borrowing->book_id = atoi(row[1]);
        borrowing->member_id = atoi(row[2]);
        borrowing->staff_id = atoi(row[3]);
        borrowing->borrow_date = atoi(row[4]);
        borrowing->due_date = atoi(row[5]);
        borrowing->return_date = atoi(row[6]);
        borrowing->status = atoi(row[7]);

        borrowing_list_add(list, borrowing);
    }

    db_free_result(result);
    return list;
}

/**
 * Find borrowings by member ID
 */
BorrowingList* borrowing_list_by_member(Database *db, int member_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM borrowings WHERE member_id=%d", member_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    BorrowingList *list = borrowing_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Borrowing *borrowing = (Borrowing*)malloc(sizeof(Borrowing));
        if (borrowing == NULL) {
            continue;
        }

        borrowing->borrowing_id = atoi(row[0]);
        borrowing->book_id = atoi(row[1]);
        borrowing->member_id = atoi(row[2]);
        borrowing->staff_id = atoi(row[3]);
        borrowing->borrow_date = atoi(row[4]);
        borrowing->due_date = atoi(row[5]);
        borrowing->return_date = atoi(row[6]);
        borrowing->status = atoi(row[7]);

        borrowing_list_add(list, borrowing);
    }

    db_free_result(result);
    return list;
}

/**
 * Find borrowings by status
 */
BorrowingList* borrowing_list_by_status(Database *db, BorrowingStatus status) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM borrowings WHERE status=%d", status);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    BorrowingList *list = borrowing_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Borrowing *borrowing = (Borrowing*)malloc(sizeof(Borrowing));
        if (borrowing == NULL) {
            continue;
        }

        borrowing->borrowing_id = atoi(row[0]);
        borrowing->book_id = atoi(row[1]);
        borrowing->member_id = atoi(row[2]);
        borrowing->staff_id = atoi(row[3]);
        borrowing->borrow_date = atoi(row[4]);
        borrowing->due_date = atoi(row[5]);
        borrowing->return_date = atoi(row[6]);
        borrowing->status = atoi(row[7]);

        borrowing_list_add(list, borrowing);
    }

    db_free_result(result);
    return list;
}

/**
 * Create a new borrowing list
 */
BorrowingList* borrowing_list_create(int initial_capacity) {
    BorrowingList *list = (BorrowingList*)malloc(sizeof(BorrowingList));
    if (list == NULL) {
        return NULL;
    }

    list->borrowings = (Borrowing**)malloc(sizeof(Borrowing*) * initial_capacity);
    if (list->borrowings == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free borrowing list resources
 */
void borrowing_list_free(BorrowingList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            borrowing_free(list->borrowings[i]);
        }
        free(list->borrowings);
        free(list);
    }
}

/**
 * Add a borrowing to the list
 */
int borrowing_list_add(BorrowingList *list, Borrowing *borrowing) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Borrowing **new_borrowings = (Borrowing**)realloc(list->borrowings, sizeof(Borrowing*) * new_capacity);
        if (new_borrowings == NULL) {
            return 0;
        }
        list->borrowings = new_borrowings;
        list->capacity = new_capacity;
    }

    list->borrowings[list->count++] = borrowing;
    return 1;
}

/**
 * Get all borrowings from the database
 */
BorrowingList* borrowing_list_all(Database *db) {
    const char *query = "SELECT * FROM borrowings";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    BorrowingList *list = borrowing_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Borrowing *borrowing = (Borrowing*)malloc(sizeof(Borrowing));
        if (borrowing == NULL) {
            continue;
        }

        borrowing->borrowing_id = atoi(row[0]);
        borrowing->book_id = atoi(row[1]);
        borrowing->member_id = atoi(row[2]);
        borrowing->staff_id = atoi(row[3]);
        borrowing->borrow_date = atoi(row[4]);
        borrowing->due_date = atoi(row[5]);
        borrowing->return_date = atoi(row[6]);
        borrowing->status = atoi(row[7]);

        borrowing_list_add(list, borrowing);
    }

    db_free_result(result);
    return list;
} 