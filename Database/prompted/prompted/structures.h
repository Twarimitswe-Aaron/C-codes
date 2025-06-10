#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <time.h>

// Book structure
typedef struct {
    int book_id;
    char title[256];
    int author_id;
    int publisher_id;
    char isbn[20];
    char genre[50];
    int year_published;
    int copies_available;
    char shelf_location[50];
} Book;

// Author structure
typedef struct {
    int author_id;
    char name[100];
    char bio[1000];
} Author;

// Publisher structure
typedef struct {
    int publisher_id;
    char name[100];
    char address[256];
    char contact_info[100];
} Publisher;

// Member structure
typedef struct {
    int member_id;
    char name[100];
    char address[256];
    char phone[20];
    char email[100];
    time_t date_joined;
    char membership_status[20];
} Member;

// Staff structure
typedef struct {
    int staff_id;
    char name[100];
    char role[50];
    char email[100];
    char phone[20];
} Staff;

// Borrowing structure
typedef struct {
    int borrowing_id;
    int book_id;
    int member_id;
    time_t borrow_date;
    time_t due_date;
    time_t return_date;
    int staff_id;
} Borrowing;

// Fine structure
typedef struct {
    int fine_id;
    int borrowing_id;
    double amount;
    int paid;
    time_t date_paid;
} Fine;

#endif // STRUCTURES_H 