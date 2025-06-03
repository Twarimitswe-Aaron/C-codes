#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRING 100
#define MAX_TEXT 500

// Structure definitions
typedef struct {
    int book_id;
    char title[MAX_STRING];
    int author_id;
    int publisher_id;
    char isbn[MAX_STRING];
    char genre[MAX_STRING];
    int year_published;
    int copies_available;
    char shelf_location[MAX_STRING];
} Book;

typedef struct {
    int author_id;
    char name[MAX_STRING];
    char bio[MAX_TEXT];
} Author;

typedef struct {
    int publisher_id;
    char name[MAX_STRING];
    char address[MAX_STRING];
    char contact_info[MAX_STRING];
} Publisher;

typedef struct {
    int member_id;
    char name[MAX_STRING];
    char address[MAX_STRING];
    char phone[MAX_STRING];
    char email[MAX_STRING];
    char date_joined[11];  // YYYY-MM-DD format
    char membership_status[MAX_STRING];
} Member;

typedef struct {
    int staff_id;
    char name[MAX_STRING];
    char role[MAX_STRING];
    char email[MAX_STRING];
    char phone[MAX_STRING];
} Staff;

typedef struct {
    int borrowing_id;
    int book_id;
    int member_id;
    char borrow_date[11];
    char due_date[11];
    char return_date[11];
    int staff_id;
} Borrowing;

typedef struct {
    int fine_id;
    int borrowing_id;
    float amount;
    int paid;
    char date_paid[11];
} Fine;

// Function declarations for file operations
void saveBook(Book book);
void saveAuthor(Author author);
void savePublisher(Publisher publisher);
void saveMember(Member member);
void saveStaff(Staff staff);
void saveBorrowing(Borrowing borrowing);
void saveFine(Fine fine);

Book* loadBooks(int* count);
Author* loadAuthors(int* count);
Publisher* loadPublishers(int* count);
Member* loadMembers(int* count);
Staff* loadStaff(int* count);
Borrowing* loadBorrowings(int* count);
Fine* loadFines(int* count);

#endif // DATABASE_H 