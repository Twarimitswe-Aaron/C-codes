#ifndef STAFF_H
#define STAFF_H

#include "database.h"
#include <time.h>

// Staff structure
typedef struct {
    int staff_id;
    char name[100];
    char role[50];
    char email[100];
    char phone[20];
    time_t created_at;
    time_t updated_at;
} Staff;

// Function declarations
Staff* staff_create(const char *name, const char *role, const char *email, const char *phone);
void staff_free(Staff *staff);

// Database operations
int staff_save(Database *db, Staff *staff);
int staff_update(Database *db, Staff *staff);
int staff_delete(Database *db, int staff_id);
Staff* staff_find_by_id(Database *db, int staff_id);
Staff* staff_find_by_email(Database *db, const char *email);

// List operations
typedef struct {
    Staff **staff_members;
    int count;
    int capacity;
} StaffList;

StaffList* staff_list_create(int initial_capacity);
void staff_list_free(StaffList *list);
int staff_list_add(StaffList *list, Staff *staff);
StaffList* staff_list_all(Database *db);
StaffList* staff_list_by_role(Database *db, const char *role);

#endif // STAFF_H 