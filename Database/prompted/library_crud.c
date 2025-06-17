#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <windows.h>
#include "library_crud.h"
#include "structures.h"

// Book CRUD Operations
void sql_addBook(sqlite3* db, const char* title, int author_id, int publisher_id, 
            const char* isbn, const char* genre, int year_published, 
            int copies_available, const char* shelf_location) {
    char* sql = "INSERT INTO books (title, author_id, publisher_id, isbn, genre, "
                "year_published, copies_available, shelf_location) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, author_id);
        sqlite3_bind_int(stmt, 3, publisher_id);
        sqlite3_bind_text(stmt, 4, isbn, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, genre, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, year_published);
        sqlite3_bind_int(stmt, 7, copies_available);
        sqlite3_bind_text(stmt, 8, shelf_location, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding book: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_updateBook(sqlite3* db, int book_id, const char* title, int author_id, 
               int publisher_id, const char* isbn, const char* genre, 
               int year_published, int copies_available, const char* shelf_location) {
    char* sql = "UPDATE books SET title=?, author_id=?, publisher_id=?, isbn=?, "
                "genre=?, year_published=?, copies_available=?, shelf_location=? "
                "WHERE book_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, title, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, author_id);
        sqlite3_bind_int(stmt, 3, publisher_id);
        sqlite3_bind_text(stmt, 4, isbn, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, genre, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, year_published);
        sqlite3_bind_int(stmt, 7, copies_available);
        sqlite3_bind_text(stmt, 8, shelf_location, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 9, book_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating book: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_deleteBook(sqlite3* db, int book_id) {
    char* sql = "DELETE FROM books WHERE book_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, book_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error deleting book: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Author CRUD Operations
void sql_addAuthor(sqlite3* db, const char* name, const char* bio) {
    char* sql = "INSERT INTO authors (name, bio) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, bio, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding author: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_updateAuthor(sqlite3* db, int author_id, const char* name, const char* bio) {
    char* sql = "UPDATE authors SET name=?, bio=? WHERE author_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, bio, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, author_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating author: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_deleteAuthor(sqlite3* db, int author_id) {
    char* sql = "DELETE FROM authors WHERE author_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, author_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error deleting author: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Publisher CRUD Operations
void sql_addPublisher(sqlite3* db, const char* name, const char* address, const char* contact_info) {
    char* sql = "INSERT INTO publishers (name, address, contact_info) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, contact_info, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding publisher: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_updatePublisher(sqlite3* db, int publisher_id, const char* name, 
                    const char* address, const char* contact_info) {
    char* sql = "UPDATE publishers SET name=?, address=?, contact_info=? WHERE publisher_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, contact_info, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, publisher_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating publisher: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_deletePublisher(sqlite3* db, int publisher_id) {
    char* sql = "DELETE FROM publishers WHERE publisher_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, publisher_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error deleting publisher: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Member CRUD Operations
void sql_addMember(sqlite3* db, const char* name, const char* address, const char* phone,
              const char* email, const char* date_joined, const char* membership_status) {
    char* sql = "INSERT INTO members (name, address, phone, email, date_joined, membership_status) "
                "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, phone, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, date_joined, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, membership_status, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding member: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_updateMember(sqlite3* db, int member_id, const char* name, const char* address,
                 const char* phone, const char* email, const char* membership_status) {
    char* sql = "UPDATE members SET name=?, address=?, phone=?, email=?, membership_status=? "
                "WHERE member_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, address, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, phone, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, membership_status, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, member_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating member: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_deleteMember(sqlite3* db, int member_id) {
    char* sql = "DELETE FROM members WHERE member_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, member_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error deleting member: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Staff CRUD Operations
void sql_addStaff(sqlite3* db, const char* name, const char* role, const char* email, const char* phone) {
    char* sql = "INSERT INTO staff (name, role, email, phone) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, role, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, phone, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding staff: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_updateStaff(sqlite3* db, int staff_id, const char* name, const char* role,
                const char* email, const char* phone) {
    char* sql = "UPDATE staff SET name=?, role=?, email=?, phone=? WHERE staff_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, role, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, phone, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, staff_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating staff: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_deleteStaff(sqlite3* db, int staff_id) {
    char* sql = "DELETE FROM staff WHERE staff_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, staff_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error deleting staff: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Borrowing Operations
void sql_addBorrowing(sqlite3* db, int book_id, int member_id, const char* borrow_date,
                 const char* due_date, int staff_id) {
    char* sql = "INSERT INTO borrowings (book_id, member_id, borrow_date, due_date, staff_id) "
                "VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, book_id);
        sqlite3_bind_int(stmt, 2, member_id);
        sqlite3_bind_text(stmt, 3, borrow_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, due_date, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, staff_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding borrowing: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_returnBook(sqlite3* db, int borrowing_id, const char* return_date) {
    char* sql = "UPDATE borrowings SET return_date=? WHERE borrowing_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, return_date, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, borrowing_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating return date: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Fine Operations
void sql_addFine(sqlite3* db, int borrowing_id, double amount) {
    char* sql = "INSERT INTO fines (borrowing_id, amount, paid) VALUES (?, ?, 0);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, borrowing_id);
        sqlite3_bind_double(stmt, 2, amount);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error adding fine: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

void sql_payFine(sqlite3* db, int fine_id, const char* date_paid) {
    char* sql = "UPDATE fines SET paid=1, date_paid=? WHERE fine_id=?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, date_paid, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, fine_id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            printf("Error updating fine payment: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(stmt);
}

// Stub implementations for missing 'get all' functions
Book* sql_getAllBooks(sqlite3* db, int* count) {
    char* sql = "SELECT * FROM books;";
    sqlite3_stmt* stmt;
    Book* books = NULL;
    *count = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Count rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*count)++;
        }
        sqlite3_reset(stmt);
        
        if (*count > 0) {
            books = (Book*)malloc(*count * sizeof(Book));
            int i = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                books[i].book_id = sqlite3_column_int(stmt, 0);
                strcpy(books[i].title, (const char*)sqlite3_column_text(stmt, 1));
                books[i].author_id = sqlite3_column_int(stmt, 2);
                books[i].publisher_id = sqlite3_column_int(stmt, 3);
                strcpy(books[i].isbn, (const char*)sqlite3_column_text(stmt, 4));
                strcpy(books[i].genre, (const char*)sqlite3_column_text(stmt, 5));
                books[i].year_published = sqlite3_column_int64(stmt, 6);
                books[i].copies_available = sqlite3_column_int(stmt, 7);
                strcpy(books[i].shelf_location, (const char*)sqlite3_column_text(stmt, 8));
                i++;
            }
        }
    }
    sqlite3_finalize(stmt);
    return books;
}

Author* sql_getAllAuthors(sqlite3* db, int* count) {
    char* sql = "SELECT * FROM authors;";
    sqlite3_stmt* stmt;
    Author* authors = NULL;
    *count = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Count rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*count)++;
        }
        sqlite3_reset(stmt);
        
        if (*count > 0) {
            authors = (Author*)malloc(*count * sizeof(Author));
            int i = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                authors[i].author_id = sqlite3_column_int(stmt, 0);
                strcpy(authors[i].name, (const char*)sqlite3_column_text(stmt, 1));
                strcpy(authors[i].bio, (const char*)sqlite3_column_text(stmt, 2));
                i++;
            }
        }
    }
    sqlite3_finalize(stmt);
    return authors;
}

Publisher* sql_getAllPublishers(sqlite3* db, int* count) {
    char* sql = "SELECT * FROM publishers;";
    sqlite3_stmt* stmt;
    Publisher* publishers = NULL;
    *count = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Count rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*count)++;
        }
        sqlite3_reset(stmt);
        
        if (*count > 0) {
            publishers = (Publisher*)malloc(*count * sizeof(Publisher));
            int i = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                publishers[i].publisher_id = sqlite3_column_int(stmt, 0);
                strcpy(publishers[i].name, (const char*)sqlite3_column_text(stmt, 1));
                strcpy(publishers[i].address, (const char*)sqlite3_column_text(stmt, 2));
                strcpy(publishers[i].contact_info, (const char*)sqlite3_column_text(stmt, 3));
                i++;
            }
        }
    }
    sqlite3_finalize(stmt);
    return publishers;
}

Member* sql_getAllMembers(sqlite3* db, int* count) {
    char* sql = "SELECT * FROM members;";
    sqlite3_stmt* stmt;
    Member* members = NULL;
    *count = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Count rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*count)++;
        }
        sqlite3_reset(stmt);
        
        if (*count > 0) {
            members = (Member*)malloc(*count * sizeof(Member));
            int i = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                members[i].member_id = sqlite3_column_int(stmt, 0);
                strcpy(members[i].name, (const char*)sqlite3_column_text(stmt, 1));
                strcpy(members[i].address, (const char*)sqlite3_column_text(stmt, 2));
                strcpy(members[i].phone, (const char*)sqlite3_column_text(stmt, 3));
                strcpy(members[i].email, (const char*)sqlite3_column_text(stmt, 4));
                members[i].date_joined = sqlite3_column_int64(stmt, 5);  // Store as time_t
                strcpy(members[i].membership_status, (const char*)sqlite3_column_text(stmt, 6));
                i++;
            }
        }
    }
    sqlite3_finalize(stmt);
    return members;
}

Staff* sql_getAllStaff(sqlite3* db, int* count) {
    char* sql = "SELECT * FROM staff;";
    sqlite3_stmt* stmt;
    Staff* staff = NULL;
    *count = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Count rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*count)++;
        }
        sqlite3_reset(stmt);
        
        if (*count > 0) {
            staff = (Staff*)malloc(*count * sizeof(Staff));
            int i = 0;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                staff[i].staff_id = sqlite3_column_int(stmt, 0);
                strcpy(staff[i].name, (const char*)sqlite3_column_text(stmt, 1));
                strcpy(staff[i].role, (const char*)sqlite3_column_text(stmt, 2));
                strcpy(staff[i].email, (const char*)sqlite3_column_text(stmt, 3));
                strcpy(staff[i].phone, (const char*)sqlite3_column_text(stmt, 4));
                i++;
            }
        }
    }
    sqlite3_finalize(stmt);
    return staff;
} 