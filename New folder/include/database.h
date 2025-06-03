#ifndef DATABASE_H
#define DATABASE_H

#include "structures.h"

// Book operations
int add_book(Book book);
int update_book(Book book);
int delete_book(int book_id);
Book* get_book(int book_id);
Book* get_all_books(int* count);

// Author operations
int add_author(Author author);
int update_author(Author author);
int delete_author(int author_id);
Author* get_author(int author_id);
Author* get_all_authors(int* count);

// Publisher operations
int add_publisher(Publisher publisher);
int update_publisher(Publisher publisher);
int delete_publisher(int publisher_id);
Publisher* get_publisher(int publisher_id);
Publisher* get_all_publishers(int* count);

// Member operations
int add_member(Member member);
int update_member(Member member);
int delete_member(int member_id);
Member* get_member(int member_id);
Member* get_all_members(int* count);

// Staff operations
int add_staff(Staff staff);
int update_staff(Staff staff);
int delete_staff(int staff_id);
Staff* get_staff(int staff_id);
Staff* get_all_staff(int* count);

// Borrowing operations
int add_borrowing(Borrowing borrowing);
int update_borrowing(Borrowing borrowing);
int delete_borrowing(int borrowing_id);
Borrowing* get_borrowing(int borrowing_id);
Borrowing* get_all_borrowings(int* count);

// Fine operations
int add_fine(Fine fine);
int update_fine(Fine fine);
int delete_fine(int fine_id);
Fine* get_fine(int fine_id);
Fine* get_all_fines(int* count);

// Database initialization
void init_database();
void close_database();

#endif 