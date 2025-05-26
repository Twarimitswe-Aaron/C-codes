#ifndef MEMBER_H
#define MEMBER_H

#include "database.h"
#include <time.h>

// Member structure
typedef struct {
    int member_id;
    char name[100];
    char address[255];
    char phone[20];
    char email[100];
    time_t date_joined;
    char membership_status[20];
    time_t created_at;
    time_t updated_at;
} Member;

// Function declarations
Member* member_create(const char *name, const char *address, const char *phone,
                     const char *email, time_t date_joined, const char *membership_status);
void member_free(Member *member);

// Database operations
int member_save(Database *db, Member *member);
int member_update(Database *db, Member *member);
int member_delete(Database *db, int member_id);
Member* member_find_by_id(Database *db, int member_id);
Member* member_find_by_email(Database *db, const char *email);

// List operations
typedef struct {
    Member **members;
    int count;
    int capacity;
} MemberList;

MemberList* member_list_create(int initial_capacity);
void member_list_free(MemberList *list);
int member_list_add(MemberList *list, Member *member);
MemberList* member_list_all(Database *db);
MemberList* member_list_by_status(Database *db, const char *status);

#endif // MEMBER_H 