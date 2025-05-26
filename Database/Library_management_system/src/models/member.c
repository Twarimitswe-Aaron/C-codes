#include "../../include/member.h"
#include <string.h>
#include <stdlib.h>

/**
 * Create a new member instance
 */
Member* member_create(const char *name, const char *address, const char *phone,
                     const char *email, time_t date_joined, const char *membership_status) {
    Member *member = (Member*)malloc(sizeof(Member));
    if (member == NULL) {
        return NULL;
    }

    // Initialize fields
    member->member_id = 0;  // Will be set by database
    strncpy(member->name, name, sizeof(member->name) - 1);
    member->name[sizeof(member->name) - 1] = '\0';
    strncpy(member->address, address, sizeof(member->address) - 1);
    member->address[sizeof(member->address) - 1] = '\0';
    strncpy(member->phone, phone, sizeof(member->phone) - 1);
    member->phone[sizeof(member->phone) - 1] = '\0';
    strncpy(member->email, email, sizeof(member->email) - 1);
    member->email[sizeof(member->email) - 1] = '\0';
    member->date_joined = date_joined;
    strncpy(member->membership_status, membership_status, sizeof(member->membership_status) - 1);
    member->membership_status[sizeof(member->membership_status) - 1] = '\0';
    member->created_at = time(NULL);
    member->updated_at = member->created_at;

    return member;
}

/**
 * Free member resources
 */
void member_free(Member *member) {
    if (member != NULL) {
        free(member);
    }
}

/**
 * Save a new member to the database
 */
int member_save(Database *db, Member *member) {
    char query[1024];
    char date_joined_str[20];
    struct tm *tm_info = localtime(&member->date_joined);
    strftime(date_joined_str, sizeof(date_joined_str), "%Y-%m-%d", tm_info);

    snprintf(query, sizeof(query),
        "INSERT INTO members (name, address, phone, email, date_joined, membership_status) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
        member->name, member->address, member->phone, member->email,
        date_joined_str, member->membership_status);

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
        member->member_id = atoi(row[0]);
    }

    db_free_result(result);
    return 1;
}

/**
 * Update an existing member in the database
 */
int member_update(Database *db, Member *member) {
    char query[1024];
    char date_joined_str[20];
    struct tm *tm_info = localtime(&member->date_joined);
    strftime(date_joined_str, sizeof(date_joined_str), "%Y-%m-%d", tm_info);

    snprintf(query, sizeof(query),
        "UPDATE members SET name='%s', address='%s', phone='%s', email='%s', "
        "date_joined='%s', membership_status='%s' WHERE member_id=%d",
        member->name, member->address, member->phone, member->email,
        date_joined_str, member->membership_status, member->member_id);

    member->updated_at = time(NULL);
    return db_execute_query(db, query);
}

/**
 * Delete a member from the database
 */
int member_delete(Database *db, int member_id) {
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM members WHERE member_id=%d", member_id);
    return db_execute_query(db, query);
}

/**
 * Find a member by ID
 */
Member* member_find_by_id(Database *db, int member_id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM members WHERE member_id=%d", member_id);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Member *member = (Member*)malloc(sizeof(Member));
    if (member == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to member structure
    member->member_id = atoi(row[0]);
    strncpy(member->name, row[1], sizeof(member->name) - 1);
    strncpy(member->address, row[2], sizeof(member->address) - 1);
    strncpy(member->phone, row[3], sizeof(member->phone) - 1);
    strncpy(member->email, row[4], sizeof(member->email) - 1);
    
    // Parse date_joined
    struct tm tm_info;
    sscanf(row[5], "%d-%d-%d", &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday);
    tm_info.tm_year -= 1900;  // struct tm years since 1900
    tm_info.tm_mon -= 1;      // struct tm months 0-11
    tm_info.tm_hour = 0;
    tm_info.tm_min = 0;
    tm_info.tm_sec = 0;
    member->date_joined = mktime(&tm_info);
    
    strncpy(member->membership_status, row[6], sizeof(member->membership_status) - 1);

    db_free_result(result);
    return member;
}

/**
 * Find a member by email
 */
Member* member_find_by_email(Database *db, const char *email) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM members WHERE email='%s'", email);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL) {
        db_free_result(result);
        return NULL;
    }

    Member *member = (Member*)malloc(sizeof(Member));
    if (member == NULL) {
        db_free_result(result);
        return NULL;
    }

    // Map database row to member structure
    member->member_id = atoi(row[0]);
    strncpy(member->name, row[1], sizeof(member->name) - 1);
    strncpy(member->address, row[2], sizeof(member->address) - 1);
    strncpy(member->phone, row[3], sizeof(member->phone) - 1);
    strncpy(member->email, row[4], sizeof(member->email) - 1);
    
    // Parse date_joined
    struct tm tm_info;
    sscanf(row[5], "%d-%d-%d", &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday);
    tm_info.tm_year -= 1900;  // struct tm years since 1900
    tm_info.tm_mon -= 1;      // struct tm months 0-11
    tm_info.tm_hour = 0;
    tm_info.tm_min = 0;
    tm_info.tm_sec = 0;
    member->date_joined = mktime(&tm_info);
    
    strncpy(member->membership_status, row[6], sizeof(member->membership_status) - 1);

    db_free_result(result);
    return member;
}

/**
 * Create a new member list
 */
MemberList* member_list_create(int initial_capacity) {
    MemberList *list = (MemberList*)malloc(sizeof(MemberList));
    if (list == NULL) {
        return NULL;
    }

    list->members = (Member**)malloc(sizeof(Member*) * initial_capacity);
    if (list->members == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

/**
 * Free member list resources
 */
void member_list_free(MemberList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            member_free(list->members[i]);
        }
        free(list->members);
        free(list);
    }
}

/**
 * Add a member to the list
 */
int member_list_add(MemberList *list, Member *member) {
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Member **new_members = (Member**)realloc(list->members, sizeof(Member*) * new_capacity);
        if (new_members == NULL) {
            return 0;
        }
        list->members = new_members;
        list->capacity = new_capacity;
    }

    list->members[list->count++] = member;
    return 1;
}

/**
 * Get all members from the database
 */
MemberList* member_list_all(Database *db) {
    const char *query = "SELECT * FROM members";
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MemberList *list = member_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Member *member = (Member*)malloc(sizeof(Member));
        if (member == NULL) {
            continue;
        }

        member->member_id = atoi(row[0]);
        strncpy(member->name, row[1], sizeof(member->name) - 1);
        strncpy(member->address, row[2], sizeof(member->address) - 1);
        strncpy(member->phone, row[3], sizeof(member->phone) - 1);
        strncpy(member->email, row[4], sizeof(member->email) - 1);
        
        // Parse date_joined
        struct tm tm_info;
        sscanf(row[5], "%d-%d-%d", &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday);
        tm_info.tm_year -= 1900;  // struct tm years since 1900
        tm_info.tm_mon -= 1;      // struct tm months 0-11
        tm_info.tm_hour = 0;
        tm_info.tm_min = 0;
        tm_info.tm_sec = 0;
        member->date_joined = mktime(&tm_info);
        
        strncpy(member->membership_status, row[6], sizeof(member->membership_status) - 1);

        member_list_add(list, member);
    }

    db_free_result(result);
    return list;
}

/**
 * Get members by status
 */
MemberList* member_list_by_status(Database *db, const char *status) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM members WHERE membership_status='%s'", status);
    
    MYSQL_RES *result = db_execute_select(db, query);
    if (result == NULL) {
        return NULL;
    }

    MemberList *list = member_list_create(10);
    if (list == NULL) {
        db_free_result(result);
        return NULL;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        Member *member = (Member*)malloc(sizeof(Member));
        if (member == NULL) {
            continue;
        }

        member->member_id = atoi(row[0]);
        strncpy(member->name, row[1], sizeof(member->name) - 1);
        strncpy(member->address, row[2], sizeof(member->address) - 1);
        strncpy(member->phone, row[3], sizeof(member->phone) - 1);
        strncpy(member->email, row[4], sizeof(member->email) - 1);
        
        // Parse date_joined
        struct tm tm_info;
        sscanf(row[5], "%d-%d-%d", &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday);
        tm_info.tm_year -= 1900;  // struct tm years since 1900
        tm_info.tm_mon -= 1;      // struct tm months 0-11
        tm_info.tm_hour = 0;
        tm_info.tm_min = 0;
        tm_info.tm_sec = 0;
        member->date_joined = mktime(&tm_info);
        
        strncpy(member->membership_status, row[6], sizeof(member->membership_status) - 1);

        member_list_add(list, member);
    }

    db_free_result(result);
    return list;
} 