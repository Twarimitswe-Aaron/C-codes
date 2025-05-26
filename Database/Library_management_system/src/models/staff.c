#include "../../include/staff.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new staff instance
 */
Staff* staff_create(const char *name, const char *role, const char *email, const char *phone) {
    Staff *staff = (Staff*)malloc(sizeof(Staff));
    if (staff == NULL) {
        return NULL;
    }

    // Initialize fields
    staff->staff_id = 0;  // Will be set by database
    strncpy(staff->name, name, sizeof(staff->name) - 1);
    staff->name[sizeof(staff->name) - 1] = '\0';
    strncpy(staff->role, role, sizeof(staff->role) - 1);
    staff->role[sizeof(staff->role) - 1] = '\0';
    strncpy(staff->email, email, sizeof(staff->email) - 1);
    staff->email[sizeof(staff->email) - 1] = '\0';
    strncpy(staff->phone, phone, sizeof(staff->phone) - 1);
    staff->phone[sizeof(staff->phone) - 1] = '\0';
    staff->created_at = time(NULL);
    staff->updated_at = staff->created_at;

    return staff;
}

/**
 * Free staff resources
 */
void staff_free(Staff *staff) {
    if (staff != NULL) {
        free(staff);
    }
}

/**
 * Save a new staff member to the database
 */
int staff_save(Database *db, Staff *staff) {
    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO staff (name, role, email, phone) VALUES ('%s', '%s', '%s', '%s')",
        staff->name, staff->role, staff->email, staff->phone);

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
        staff->staff_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing staff member in the database
 */
int staff_update(Database *db, Staff *staff) {
    char query[1024];
    snprintf(query, sizeof(query),
        "UPDATE staff SET name='%s', role='%s', email='%s', phone='%s' WHERE staff_id=%d",
        staff->name, staff->role, staff->email, staff->phone, staff->staff_id);

    staff->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete a staff member from the database
 */
int staff_delete(Database *db, int staff_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM staff WHERE staff_id=%d", staff_id);
    return db_execute_query(db, query);
}

/**
 * Find a staff member by ID
 */
Staff* staff_find_by_id(Database *db, int staff_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM staff WHERE staff_id=%d", staff_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Staff *staff = (Staff*)malloc(sizeof(Staff));
    if (staff == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to staff structure
    staff->staff_id = atoi(row[0]);
    strncpy(staff->name, row[1], sizeof(staff->name) - 1);
    strncpy(staff->role, row[2], sizeof(staff->role) - 1);
    strncpy(staff->email, row[3], sizeof(staff->email) - 1);
    strncpy(staff->phone, row[4], sizeof(staff->phone) - 1);

    db_free_result(result);
    return staff;
}

/**
 * Find a staff member by email
 */
Staff* staff_find_by_email(Database *db, const char *email) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM staff WHERE email='%s'", email);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Staff *staff = (Staff*)malloc(sizeof(Staff));
    if (staff == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to staff structure
    staff->staff_id = atoi(row[0]);
    strncpy(staff->name, row[1], sizeof(staff->name) - 1);
    strncpy(staff->role, row[2], sizeof(staff->role) - 1);
    strncpy(staff->email, row[3], sizeof(staff->email) - 1);
    strncpy(staff->phone, row[4], sizeof(staff->phone) - 1);

    db_free_result(result);
    return staff;
}

/**
 * Create a new staff list
 */
StaffList* staff_list_create(int initial_capacity) {
    StaffList *list = (StaffList*)malloc(sizeof(StaffList));
    if (list == NULL) {
        return NULL;
    }

    list->staff_members = (Staff**)malloc(sizeof(Staff*) * initial_capacity);
    if (list->staff_members == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free staff list resources
 */
void staff_list_free(StaffList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            staff_free(list->staff_members[i]);
        }
        free(list->staff_members);
        free(list);
    }
}

/**
 * Add a staff member to the list
 */
int staff_list_add(StaffList *list, Staff *staff) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Staff **new_staff = (Staff**)realloc(list->staff_members, sizeof(Staff*) * new_capacity);
        if (new_staff == NULL) {
            return 0;
        }
        list->staff_members = new_staff;
        list->capacity = new_capacity;
    }

    list->staff_members[list->count++] = staff;
    return 1;
}

/**
 * Get all staff members from the database
 */
StaffList* staff_list_all(Database *db) {
    const char *query = "SELECT * FROM staff";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    StaffList *list = staff_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Staff *staff = (Staff*)malloc(sizeof(Staff));
        if (staff == NULL) {
            continue;
        }

        staff->staff_id = atoi(row[0]);
        strncpy(staff->name, row[1], sizeof(staff->name) - 1);
        strncpy(staff->role, row[2], sizeof(staff->role) - 1);
        strncpy(staff->email, row[3], sizeof(staff->email) - 1);
        strncpy(staff->phone, row[4], sizeof(staff->phone) - 1);

        staff_list_add(list, staff);
    }

    db_free_result(result);
    return list;
}

/**
 * Get staff members by role
 */
StaffList* staff_list_by_role(Database *db, const char *role) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM staff WHERE role='%s'", role);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    StaffList *list = staff_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Staff *staff = (Staff*)malloc(sizeof(Staff));
        if (staff == NULL) {
            continue;
        }

        staff->staff_id = atoi(row[0]);
        strncpy(staff->name, row[1], sizeof(staff->name) - 1);
        strncpy(staff->role, row[2], sizeof(staff->role) - 1);
        strncpy(staff->email, row[3], sizeof(staff->email) - 1);
        strncpy(staff->phone, row[4], sizeof(staff->phone) - 1);

        staff_list_add(list, staff);
    }

    db_free_result(result);
    return list;
} 