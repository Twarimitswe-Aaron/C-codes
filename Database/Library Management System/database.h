#ifndef DATABASE_H
#define DATABASE_H

// Standard library includes
#include <stdio.h>    // For file operations
#include <stdlib.h>   // For memory allocation
#include <string.h>   // For string operations
#include <time.h>     // For date/time operations

// Constants for string lengths
#define MAX_STRING_LENGTH 256   // Maximum length for regular strings
#define MAX_TEXT_LENGTH 1024    // Maximum length for longer text (like bios)

// File paths for our text files
#define BOOKS_FILE "data/books.txt"
#define MEMBERS_FILE "data/members.txt"
#define BORROWINGS_FILE "data/borrowings.txt"

// Structure definitions for our database entities

// Author structure - stores information about book authors
typedef struct {
    int author_id;                          // Unique identifier for the author
    char name[MAX_STRING_LENGTH];           // Author's name
    char bio[MAX_TEXT_LENGTH];              // Author's biography
} Author;

// Publisher structure - stores information about book publishers
typedef struct {
    int publisher_id;                       // Unique identifier for the publisher
    char name[MAX_STRING_LENGTH];           // Publisher's name
    char address[MAX_STRING_LENGTH];        // Publisher's address
    char contact_info[MAX_STRING_LENGTH];   // Publisher's contact information
} Publisher;

// Book structure - stores information about books
typedef struct {
    int book_id;
    char title[MAX_STRING_LENGTH];
    char author[MAX_STRING_LENGTH];  // Changed from author_id to author
    char isbn[MAX_STRING_LENGTH];
    int copies_available;
} Book;

// Member structure - stores information about library members
typedef struct {
    int member_id;
    char name[MAX_STRING_LENGTH];
    char phone[MAX_STRING_LENGTH];
    char email[MAX_STRING_LENGTH];
} Member;

// Staff structure - stores information about library staff
typedef struct {
    int staff_id;                          // Unique identifier for the staff
    char name[MAX_STRING_LENGTH];          // Staff member's name
    char role[MAX_STRING_LENGTH];          // Staff member's role
    char email[MAX_STRING_LENGTH];         // Staff member's email
    char phone[MAX_STRING_LENGTH];         // Staff member's phone
} Staff;

// Borrowing structure - tracks book borrowing transactions
typedef struct {
    int borrowing_id;
    int book_id;
    int member_id;
    char borrow_date[11];
    char due_date[11];
    char return_date[11];
} Borrowing;

// Fine structure - tracks fines for late returns
typedef struct {
    int fine_id;                           // Unique identifier for the fine
    int borrowing_id;                      // Foreign key to Borrowings table
    float amount;                          // Fine amount
    int paid;                              // Whether fine has been paid
    char date_paid[11];                    // Date fine was paid
} Fine;

// Function declarations for database operations

// Book operations
int add_book(Book book);                   // Add a new book
int update_book(Book book);                // Update existing book
int delete_book(int book_id);              // Delete a book
Book* get_book(int book_id);               // Get a single book
Book* get_all_books(int* count);           // Get all books

// Author operations
int add_author(Author author);             // Add a new author
int update_author(Author author);          // Update existing author
int delete_author(int author_id);          // Delete an author
Author* get_author(int author_id);         // Get a single author
Author* get_all_authors(int* count);       // Get all authors

// Publisher operations
int add_publisher(Publisher publisher);    // Add a new publisher
int update_publisher(Publisher publisher); // Update existing publisher
int delete_publisher(int publisher_id);    // Delete a publisher
Publisher* get_publisher(int publisher_id); // Get a single publisher
Publisher* get_all_publishers(int* count); // Get all publishers

// Member operations
int add_member(Member member);             // Add a new member
int update_member(Member member);          // Update existing member
int delete_member(int member_id);          // Delete a member
Member* get_member(int member_id);         // Get a single member
Member* get_all_members(int* count);       // Get all members

// Staff operations
int add_staff(Staff staff);                // Add a new staff member
int update_staff(Staff staff);             // Update existing staff
int delete_staff(int staff_id);            // Delete a staff member
Staff* get_staff(int staff_id);            // Get a single staff member
Staff* get_all_staff(int* count);          // Get all staff members

// Borrowing operations
int add_borrowing(Borrowing borrowing);    // Add a new borrowing
int update_borrowing(Borrowing borrowing); // Update existing borrowing
int delete_borrowing(int borrowing_id);    // Delete a borrowing
Borrowing* get_borrowing(int borrowing_id); // Get a single borrowing
Borrowing* get_all_borrowings(int* count); // Get all borrowings

// Fine operations
int add_fine(Fine fine);                   // Add a new fine
int update_fine(Fine fine);                // Update existing fine
int delete_fine(int fine_id);              // Delete a fine
Fine* get_fine(int fine_id);               // Get a single fine
Fine* get_all_fines(int* count);           // Get all fines

// Database initialization
int init_files();

// Table creation functions
int create_books_table();
int create_members_table();
int create_borrowings_table();

// Display functions
void display_all_books();
void display_all_members();
void display_all_borrowings();

#endif // DATABASE_H 