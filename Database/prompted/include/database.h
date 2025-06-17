#ifndef DATABASE_H
#define DATABASE_H

#include "structures.h"
#include <sqlite3.h>

// Book functions
void sql_addBook(sqlite3* db, const char* title, int author_id, int publisher_id, 
            const char* isbn, const char* genre, int year_published, 
            int copies_available, const char* shelf_location);
void sql_updateBook(sqlite3* db, int book_id, const char* title, int author_id, 
               int publisher_id, const char* isbn, const char* genre, 
               int year_published, int copies_available, const char* shelf_location);
void sql_deleteBook(sqlite3* db, int book_id);

// Author functions
void sql_addAuthor(sqlite3* db, const char* name, const char* bio);
void sql_updateAuthor(sqlite3* db, int author_id, const char* name, const char* bio);
void sql_deleteAuthor(sqlite3* db, int author_id);

// Publisher functions
void sql_addPublisher(sqlite3* db, const char* name, const char* address, const char* contact_info);
void sql_updatePublisher(sqlite3* db, int publisher_id, const char* name, 
                    const char* address, const char* contact_info);
void sql_deletePublisher(sqlite3* db, int publisher_id);

// Member functions
void sql_addMember(sqlite3* db, const char* name, const char* address, const char* phone,
              const char* email, const char* date_joined, const char* membership_status);
void sql_updateMember(sqlite3* db, int member_id, const char* name, const char* address,
                 const char* phone, const char* email, const char* membership_status);
void sql_deleteMember(sqlite3* db, int member_id);

// Staff functions
void sql_addStaff(sqlite3* db, const char* name, const char* role, const char* email, const char* phone);
void sql_updateStaff(sqlite3* db, int staff_id, const char* name, const char* role,
                const char* email, const char* phone);
void sql_deleteStaff(sqlite3* db, int staff_id);

// Borrowing functions
void sql_addBorrowing(sqlite3* db, int book_id, int member_id, const char* borrow_date,
                 const char* due_date, int staff_id);
void sql_returnBook(sqlite3* db, int borrowing_id, const char* return_date);

// Fine functions
void sql_addFine(sqlite3* db, int borrowing_id, double amount);
void sql_payFine(sqlite3* db, int fine_id, const char* date_paid);

#endif // DATABASE_H 