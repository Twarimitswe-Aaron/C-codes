#ifndef LIBRARY_CRUD_H
#define LIBRARY_CRUD_H

#include <sqlite3.h>
#include "structures.h"

// Book CRUD Operations
void sql_addBook(sqlite3* db, const char* title, int author_id, int publisher_id, 
            const char* isbn, const char* genre, int year_published, 
            int copies_available, const char* shelf_location);
void sql_updateBook(sqlite3* db, int book_id, const char* title, int author_id, 
               int publisher_id, const char* isbn, const char* genre, 
               int year_published, int copies_available, const char* shelf_location);
void sql_deleteBook(sqlite3* db, int book_id);
Book* sql_getAllBooks(sqlite3* db, int* count);

// Author CRUD Operations
void sql_addAuthor(sqlite3* db, const char* name, const char* bio);
void sql_updateAuthor(sqlite3* db, int author_id, const char* name, const char* bio);
void sql_deleteAuthor(sqlite3* db, int author_id);
Author* sql_getAllAuthors(sqlite3* db, int* count);

// Publisher CRUD Operations
void sql_addPublisher(sqlite3* db, const char* name, const char* address, const char* contact_info);
void sql_updatePublisher(sqlite3* db, int publisher_id, const char* name, 
                    const char* address, const char* contact_info);
void sql_deletePublisher(sqlite3* db, int publisher_id);
Publisher* sql_getAllPublishers(sqlite3* db, int* count);

// Member CRUD Operations
void sql_addMember(sqlite3* db, const char* name, const char* address, const char* phone,
              const char* email, const char* date_joined, const char* membership_status);
void sql_updateMember(sqlite3* db, int member_id, const char* name, const char* address,
                 const char* phone, const char* email, const char* membership_status);
void sql_deleteMember(sqlite3* db, int member_id);
Member* sql_getAllMembers(sqlite3* db, int* count);

// Staff CRUD Operations
void sql_addStaff(sqlite3* db, const char* name, const char* role, const char* email, const char* phone);
void sql_updateStaff(sqlite3* db, int staff_id, const char* name, const char* role,
                const char* email, const char* phone);
void sql_deleteStaff(sqlite3* db, int staff_id);
Staff* sql_getAllStaff(sqlite3* db, int* count);

#endif // LIBRARY_CRUD_H 