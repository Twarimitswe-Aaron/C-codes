#ifndef FINE_H
#define FINE_H

#include "database.h"
#include <time.h>

// Fine status enum
typedef enum {
    PENDING = 0,
    PAID = 1,
    WAIVED = 2
} FineStatus;

// Fine structure
typedef struct {
    int fine_id;
    int borrowing_id;
    double amount;
    time_t due_date;
    time_t paid_date;
    FineStatus status;
    time_t created_at;
    time_t updated_at;
} Fine;

// Fine list structure
typedef struct {
    Fine **fines;
    int count;
    int capacity;
} FineList;

// Create a new fine instance
Fine* fine_create(int borrowing_id, double amount, time_t due_date);

// Free fine resources
void fine_free(Fine *fine);

// Save a new fine to the database
int fine_save(Database *db, Fine *fine);

// Update an existing fine in the database
int fine_update(Database *db, Fine *fine);

// Delete a fine from the database
int fine_delete(Database *db, int fine_id);

// Find a fine by ID
Fine* fine_find_by_id(Database *db, int fine_id);

// Find fines by borrowing ID
FineList* fine_list_by_borrowing(Database *db, int borrowing_id);

// Find fines by status
FineList* fine_list_by_status(Database *db, FineStatus status);

// Create a new fine list
FineList* fine_list_create(int initial_capacity);

// Free fine list resources
void fine_list_free(FineList *list);

// Add a fine to the list
int fine_list_add(FineList *list, Fine *fine);

// Get all fines from the database
FineList* fine_list_all(Database *db);

#endif // FINE_H 