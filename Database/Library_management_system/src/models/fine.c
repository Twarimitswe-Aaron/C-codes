#include "../../include/fine.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new fine instance
 */
Fine* fine_create(int borrowing_id, double amount, time_t due_date) {
    Fine *fine = (Fine*)malloc(sizeof(Fine));
    if (fine == NULL) {
        return NULL;
    }

    // Initialize fields
    fine->fine_id = 0;  // Will be set by database
    fine->borrowing_id = borrowing_id;
    fine->amount = amount;
    fine->due_date = due_date;
    fine->paid_date = 0;  // Not paid yet
    fine->status = PENDING;
    fine->created_at = time(NULL);
    fine->updated_at = fine->created_at;

    return fine;
}

/**
 * Free fine resources
 */
void fine_free(Fine *fine) {
    if (fine != NULL) {
        free(fine);
    }
}

/**
 * Save a new fine to the database
 */
int fine_save(Database *db, Fine *fine) {
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO fines (borrowing_id, amount, due_date, status) "
        "VALUES (%d, %.2f, FROM_UNIXTIME(%lld), %d)",
        fine->borrowing_id, fine->amount, fine->due_date, fine->status);

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
        fine->fine_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing fine in the database
 */
int fine_update(Database *db, Fine *fine) {
    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE fines SET borrowing_id=%d, amount=%.2f, "
        "due_date=FROM_UNIXTIME(%lld), paid_date=FROM_UNIXTIME(%lld), "
        "status=%d WHERE fine_id=%d",
        fine->borrowing_id, fine->amount, fine->due_date,
        fine->paid_date, fine->status, fine->fine_id);

    fine->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete a fine from the database
 */
int fine_delete(Database *db, int fine_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM fines WHERE fine_id=%d", fine_id);
    return db_execute_query(db, query);
}

/**
 * Find a fine by ID
 */
Fine* fine_find_by_id(Database *db, int fine_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM fines WHERE fine_id=%d", fine_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Fine *fine = (Fine*)malloc(sizeof(Fine));
    if (fine == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to fine structure
    fine->fine_id = atoi(row[0]);
    fine->borrowing_id = atoi(row[1]);
    fine->amount = atof(row[2]);
    fine->due_date = atoi(row[3]);
    fine->paid_date = atoi(row[4]);
    fine->status = atoi(row[5]);

    db_free_result(result);
    return fine;
}

/**
 * Find fines by borrowing ID
 */
FineList* fine_list_by_borrowing(Database *db, int borrowing_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM fines WHERE borrowing_id=%d", borrowing_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    FineList *list = fine_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Fine *fine = (Fine*)malloc(sizeof(Fine));
        if (fine == NULL) {
            continue;
        }

        fine->fine_id = atoi(row[0]);
        fine->borrowing_id = atoi(row[1]);
        fine->amount = atof(row[2]);
        fine->due_date = atoi(row[3]);
        fine->paid_date = atoi(row[4]);
        fine->status = atoi(row[5]);

        fine_list_add(list, fine);
    }

    db_free_result(result);
    return list;
}

/**
 * Find fines by status
 */
FineList* fine_list_by_status(Database *db, FineStatus status) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM fines WHERE status=%d", status);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    FineList *list = fine_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Fine *fine = (Fine*)malloc(sizeof(Fine));
        if (fine == NULL) {
            continue;
        }

        fine->fine_id = atoi(row[0]);
        fine->borrowing_id = atoi(row[1]);
        fine->amount = atof(row[2]);
        fine->due_date = atoi(row[3]);
        fine->paid_date = atoi(row[4]);
        fine->status = atoi(row[5]);

        fine_list_add(list, fine);
    }

    db_free_result(result);
    return list;
}

/**
 * Create a new fine list
 */
FineList* fine_list_create(int initial_capacity) {
    FineList *list = (FineList*)malloc(sizeof(FineList));
    if (list == NULL) {
        return NULL;
    }

    list->fines = (Fine**)malloc(sizeof(Fine*) * initial_capacity);
    if (list->fines == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free fine list resources
 */
void fine_list_free(FineList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            fine_free(list->fines[i]);
        }
        free(list->fines);
        free(list);
    }
}

/**
 * Add a fine to the list
 */
int fine_list_add(FineList *list, Fine *fine) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Fine **new_fines = (Fine**)realloc(list->fines, sizeof(Fine*) * new_capacity);
        if (new_fines == NULL) {
            return 0;
        }
        list->fines = new_fines;
        list->capacity = new_capacity;
    }

    list->fines[list->count++] = fine;
    return 1;
}

/**
 * Get all fines from the database
 */
FineList* fine_list_all(Database *db) {
    const char *query = "SELECT * FROM fines";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    FineList *list = fine_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Fine *fine = (Fine*)malloc(sizeof(Fine));
        if (fine == NULL) {
            continue;
        }

        fine->fine_id = atoi(row[0]);
        fine->borrowing_id = atoi(row[1]);
        fine->amount = atof(row[2]);
        fine->due_date = atoi(row[3]);
        fine->paid_date = atoi(row[4]);
        fine->status = atoi(row[5]);

        fine_list_add(list, fine);
    }

    db_free_result(result);
    return list;
} 