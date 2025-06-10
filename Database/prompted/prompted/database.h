#ifndef DATABASE_H
#define DATABASE_H

#include "structures.h"

// Book functions
Book* getAllBooks(int* count);
Book* getBook(int id);
int addBook(Book book);
int updateBook(Book book);
int deleteBook(int id);

// Author functions
Author* getAllAuthors(int* count);
Author* getAuthor(int id);
int addAuthor(Author author);
int updateAuthor(Author author);
int deleteAuthor(int id);

// Publisher functions
Publisher* getAllPublishers(int* count);
Publisher* getPublisher(int id);
int addPublisher(Publisher publisher);
int updatePublisher(Publisher publisher);
int deletePublisher(int id);

// Member functions
Member* getAllMembers(int* count);
Member* getMember(int id);
int addMember(Member member);
int updateMember(Member member);
int deleteMember(int id);

// Staff functions
Staff* getAllStaff(int* count);
Staff* getStaff(int id);
int addStaff(Staff staff);
int updateStaff(Staff staff);
int deleteStaff(int id);

// Borrowing functions
Borrowing* getAllBorrowings(int* count);
Borrowing* getBorrowing(int id);
int addBorrowing(Borrowing borrowing);
int updateBorrowing(Borrowing borrowing);
int deleteBorrowing(int id);

// Fine functions
Fine* getAllFines(int* count);
Fine* getFine(int id);
int addFine(Fine fine);
int updateFine(Fine fine);
int deleteFine(int id);

// Helper functions
int getNextId(const char* filename);

#endif // DATABASE_H 