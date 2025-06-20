#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "database.h"
#include "date_utils.h"

// Helper function to get next available ID
int getNextId(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return 1;

    int maxId = 0;
    int id;
    while (fread(&id, sizeof(int), 1, file) == 1) {
        if (id > maxId) maxId = id;
    }
    fclose(file);
    return maxId + 1;
}

// Book operations
int addBook(Book book) {
    // Check if book with same ID already exists
    if (book.book_id > 0) {
        Book* existingBook = getBook(book.book_id);
        if (existingBook) {
            printf("Error: Book with ID %d already exists.\n", book.book_id);
            free(existingBook);
            return 0;
        }
    }

    // Referential integrity: check author and publisher exist
    if (book.author_id <= 0 || getAuthor(book.author_id) == NULL) {
        printf("Error: Author ID %d does not exist.\n", book.author_id);
        return 0;
    }
    if (book.publisher_id <= 0 || getPublisher(book.publisher_id) == NULL) {
        printf("Error: Publisher ID %d does not exist.\n", book.publisher_id);
        return 0;
    }
    // Non-null checks
    if (strlen(book.title) == 0 || strlen(book.isbn) == 0 || strlen(book.genre) == 0 || strlen(book.shelf_location) == 0 || book.year_published == 0 || book.copies_available == 0) {
        printf("Error: All fields must be filled and non-zero.\n");
        return 0;
    }

    // If book_id is not set, generate a new one
    if (book.book_id <= 0) {
        book.book_id = getNextId("database/books.dat");
    }

    FILE* file = fopen("database/books.dat", "ab");
    if (!file) return 0;
    
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);
    return 1;
}

int updateBook(Book book) {
    // Referential integrity: check author and publisher exist
    if (book.author_id <= 0 || getAuthor(book.author_id) == NULL) {
        printf("Error: Author ID %d does not exist.\n", book.author_id);
        return 0;
    }
    if (book.publisher_id <= 0 || getPublisher(book.publisher_id) == NULL) {
        printf("Error: Publisher ID %d does not exist.\n", book.publisher_id);
        return 0;
    }
    // Non-null checks
    if (strlen(book.title) == 0 || strlen(book.isbn) == 0 || strlen(book.genre) == 0 || strlen(book.shelf_location) == 0 || book.year_published == 0 || book.copies_available == 0) {
        printf("Error: All fields must be filled and non-zero.\n");
        return 0;
    }
    FILE* file = fopen("database/books.dat", "rb+");
    if (!file) return 0;
    Book temp;
    while (fread(&temp, sizeof(Book), 1, file) == 1) {
        if (temp.book_id == book.book_id) {
            long long offset = -(long long)sizeof(Book);
            fseek(file, offset, SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deleteBook(int book_id) {
    FILE* file = fopen("database/books.dat", "rb");
    if (!file) return 0;

    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file) == 1) {
        if (book.book_id != book_id) {
            fwrite(&book, sizeof(Book), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("database/books.dat");
    rename("database/temp.dat", "database/books.dat");

    return found;
}

Book* getBook(int book_id) {
    FILE* file = fopen("database/books.dat", "rb");
    if (!file) return NULL;

    Book* book = (Book*)malloc(sizeof(Book));
    while (fread(book, sizeof(Book), 1, file) == 1) {
        if (book->book_id == book_id) {
            fclose(file);
            return book;
        }
    }

    fclose(file);
    free(book);
    return NULL;
}

Book* getAllBooks(int* count) {
    FILE* file = fopen("database/books.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    // Count records
    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Book);
    fseek(file, 0, SEEK_SET);

    Book* books = (Book*)malloc(*count * sizeof(Book));
    fread(books, sizeof(Book), *count, file);
    fclose(file);

    return books;
}

// Author operations
int addAuthor(Author author) {
    if (strlen(author.name) == 0) {
        printf("Error: Author name cannot be empty.\n");
        return 0;
    }
    FILE* file = fopen("database/authors.dat", "ab");
    if (!file) return 0;
    author.author_id = getNextId("database/authors.dat");
    fwrite(&author, sizeof(Author), 1, file);
    fclose(file);
    return 1;
}

int updateAuthor(Author author) {
    if (strlen(author.name) == 0) {
        printf("Error: Author name cannot be empty.\n");
        return 0;
    }
    FILE* file = fopen("database/authors.dat", "rb+");
    if (!file) return 0;
    Author temp;
    while (fread(&temp, sizeof(Author), 1, file) == 1) {
        if (temp.author_id == author.author_id) {
            long long offset = -(long long)sizeof(Author);
            fseek(file, offset, SEEK_CUR);
            fwrite(&author, sizeof(Author), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deleteAuthor(int author_id) {
    // Check referential integrity: is this author referenced by any book?
    int book_count = 0;
    Book* books = getAllBooks(&book_count);
    if (books) {
        for (int i = 0; i < book_count; i++) {
            if (books[i].author_id == author_id) {
                printf("Error: Cannot delete author. There are books referencing this author.\n");
                free(books);
                return 0;
            }
        }
        free(books);
    }
    FILE* file = fopen("database/authors.dat", "rb");
    if (!file) return 0;
    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }
    Author author;
    int found = 0;
    while (fread(&author, sizeof(Author), 1, file) == 1) {
        if (author.author_id != author_id) {
            fwrite(&author, sizeof(Author), 1, tempFile);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("database/authors.dat");
    rename("database/temp.dat", "database/authors.dat");
    return found;
}

Author* getAuthor(int author_id) {
    FILE* file = fopen("database/authors.dat", "rb");
    if (!file) return NULL;

    Author* author = (Author*)malloc(sizeof(Author));
    while (fread(author, sizeof(Author), 1, file) == 1) {
        if (author->author_id == author_id) {
            fclose(file);
            return author;
        }
    }

    fclose(file);
    free(author);
    return NULL;
}

Author* getAllAuthors(int* count) {
    FILE* file = fopen("database/authors.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Author);
    fseek(file, 0, SEEK_SET);

    Author* authors = (Author*)malloc(*count * sizeof(Author));
    fread(authors, sizeof(Author), *count, file);
    fclose(file);

    return authors;
}

// Publisher operations
int addPublisher(Publisher publisher) {
    if (strlen(publisher.name) == 0) {
        printf("Error: Publisher name cannot be empty.\n");
        return 0;
    }
    FILE* file = fopen("database/publishers.dat", "ab");
    if (!file) return 0;

    publisher.publisher_id = getNextId("database/publishers.dat");
    fwrite(&publisher, sizeof(Publisher), 1, file);
    fclose(file);
    return 1;
}

int updatePublisher(Publisher publisher) {
    if (strlen(publisher.name) == 0) {
        printf("Error: Publisher name cannot be empty.\n");
        return 0;
    }
    FILE* file = fopen("database/publishers.dat", "rb+");
    if (!file) return 0;

    Publisher temp;
    while (fread(&temp, sizeof(Publisher), 1, file) == 1) {
        if (temp.publisher_id == publisher.publisher_id) {
            long long offset = -(long long)sizeof(Publisher);
            fseek(file, offset, SEEK_CUR);
            fwrite(&publisher, sizeof(Publisher), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deletePublisher(int publisher_id) {
    // Check referential integrity: is this publisher referenced by any book?
    int book_count = 0;
    Book* books = getAllBooks(&book_count);
    if (books) {
        for (int i = 0; i < book_count; i++) {
            if (books[i].publisher_id == publisher_id) {
                printf("Error: Cannot delete publisher. There are books referencing this publisher.\n");
                free(books);
                return 0;
            }
        }
        free(books);
    }
    FILE* file = fopen("database/publishers.dat", "rb");
    if (!file) return 0;
    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }
    Publisher publisher;
    int found = 0;
    while (fread(&publisher, sizeof(Publisher), 1, file) == 1) {
        if (publisher.publisher_id != publisher_id) {
            fwrite(&publisher, sizeof(Publisher), 1, tempFile);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("database/publishers.dat");
    rename("database/temp.dat", "database/publishers.dat");
    return found;
}

Publisher* getPublisher(int publisher_id) {
    FILE* file = fopen("database/publishers.dat", "rb");
    if (!file) return NULL;

    Publisher* publisher = (Publisher*)malloc(sizeof(Publisher));
    while (fread(publisher, sizeof(Publisher), 1, file) == 1) {
        if (publisher->publisher_id == publisher_id) {
            fclose(file);
            return publisher;
        }
    }

    fclose(file);
    free(publisher);
    return NULL;
}

Publisher* getAllPublishers(int* count) {
    FILE* file = fopen("database/publishers.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Publisher);
    fseek(file, 0, SEEK_SET);

    Publisher* publishers = (Publisher*)malloc(*count * sizeof(Publisher));
    fread(publishers, sizeof(Publisher), *count, file);
    fclose(file);

    return publishers;
}

// Member operations
int addMember(Member member) {
    if (strlen(member.name) == 0 || strlen(member.address) == 0 || strlen(member.phone) == 0 || strlen(member.email) == 0) {
        printf("Error: All member fields must be filled.\n");
        return 0;
    }
    FILE* file = fopen("database/members.dat", "ab");
    if (!file) return 0;

    member.member_id = getNextId("database/members.dat");
    fwrite(&member, sizeof(Member), 1, file);
    fclose(file);
    return 1;
}

int updateMember(Member member) {
    if (strlen(member.name) == 0 || strlen(member.address) == 0 || strlen(member.phone) == 0 || strlen(member.email) == 0) {
        printf("Error: All member fields must be filled.\n");
        return 0;
    }
    FILE* file = fopen("database/members.dat", "rb+");
    if (!file) return 0;

    Member temp;
    while (fread(&temp, sizeof(Member), 1, file) == 1) {
        if (temp.member_id == member.member_id) {
            long long offset = -(long long)sizeof(Member);
            fseek(file, offset, SEEK_CUR);
            fwrite(&member, sizeof(Member), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deleteMember(int member_id) {
    // Check referential integrity: is this member referenced by any borrowing?
    int borrowing_count = 0;
    Borrowing* borrowings = getAllBorrowings(&borrowing_count);
    if (borrowings) {
        for (int i = 0; i < borrowing_count; i++) {
            if (borrowings[i].member_id == member_id) {
                printf("Error: Cannot delete member. There are borrowings referencing this member.\n");
                free(borrowings);
                return 0;
            }
        }
        free(borrowings);
    }
    FILE* file = fopen("database/members.dat", "rb");
    if (!file) return 0;

    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }

    Member member;
    int found = 0;
    while (fread(&member, sizeof(Member), 1, file) == 1) {
        if (member.member_id != member_id) {
            fwrite(&member, sizeof(Member), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("database/members.dat");
    rename("database/temp.dat", "database/members.dat");

    return found;
}

Member* getMember(int member_id) {
    FILE* file = fopen("database/members.dat", "rb");
    if (!file) return NULL;

    Member* member = (Member*)malloc(sizeof(Member));
    while (fread(member, sizeof(Member), 1, file) == 1) {
        if (member->member_id == member_id) {
            fclose(file);
            return member;
        }
    }

    fclose(file);
    free(member);
    return NULL;
}

Member* getAllMembers(int* count) {
    FILE* file = fopen("database/members.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Member);
    fseek(file, 0, SEEK_SET);

    Member* members = (Member*)malloc(*count * sizeof(Member));
    fread(members, sizeof(Member), *count, file);
    fclose(file);

    return members;
}

// Staff operations
int addStaff(Staff staff) {
    if (strlen(staff.name) == 0 || strlen(staff.role) == 0 || strlen(staff.email) == 0 || strlen(staff.phone) == 0) {
        printf("Error: All staff fields must be filled.\n");
        return 0;
    }
    FILE* file = fopen("database/staff.dat", "ab");
    if (!file) return 0;

    staff.staff_id = getNextId("database/staff.dat");
    fwrite(&staff, sizeof(Staff), 1, file);
    fclose(file);
    return 1;
}

int updateStaff(Staff staff) {
    if (strlen(staff.name) == 0 || strlen(staff.role) == 0 || strlen(staff.email) == 0 || strlen(staff.phone) == 0) {
        printf("Error: All staff fields must be filled.\n");
        return 0;
    }
    FILE* file = fopen("database/staff.dat", "rb+");
    if (!file) return 0;

    Staff temp;
    while (fread(&temp, sizeof(Staff), 1, file) == 1) {
        if (temp.staff_id == staff.staff_id) {
            long long offset = -(long long)sizeof(Staff);
            fseek(file, offset, SEEK_CUR);
            fwrite(&staff, sizeof(Staff), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deleteStaff(int staff_id) {
    // Check referential integrity: is this staff member referenced by any borrowing?
    int borrowing_count = 0;
    Borrowing* borrowings = getAllBorrowings(&borrowing_count);
    if (borrowings) {
        for (int i = 0; i < borrowing_count; i++) {
            if (borrowings[i].staff_id == staff_id) {
                printf("Error: Cannot delete staff member. There are borrowings referencing this staff member.\n");
                free(borrowings);
                return 0;
            }
        }
        free(borrowings);
    }
    FILE* file = fopen("database/staff.dat", "rb");
    if (!file) return 0;

    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }

    Staff staff;
    int found = 0;
    while (fread(&staff, sizeof(Staff), 1, file) == 1) {
        if (staff.staff_id != staff_id) {
            fwrite(&staff, sizeof(Staff), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("database/staff.dat");
    rename("database/temp.dat", "database/staff.dat");

    return found;
}

Staff* getStaff(int staff_id) {
    FILE* file = fopen("database/staff.dat", "rb");
    if (!file) return NULL;

    Staff* staff = (Staff*)malloc(sizeof(Staff));
    while (fread(staff, sizeof(Staff), 1, file) == 1) {
        if (staff->staff_id == staff_id) {
            fclose(file);
            return staff;
        }
    }

    fclose(file);
    free(staff);
    return NULL;
}

Staff* getAllStaff(int* count) {
    FILE* file = fopen("database/staff.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Staff);
    fseek(file, 0, SEEK_SET);

    Staff* staff = (Staff*)malloc(*count * sizeof(Staff));
    fread(staff, sizeof(Staff), *count, file);
    fclose(file);

    return staff;
}

// Borrowing operations
int addBorrowing(Borrowing borrowing) {
    // Referential integrity: check book and member exist
    if (borrowing.book_id <= 0 || getBook(borrowing.book_id) == NULL) {
        printf("Error: Book ID %d does not exist.\n", borrowing.book_id);
        return 0;
    }
    if (borrowing.member_id <= 0 || getMember(borrowing.member_id) == NULL) {
        printf("Error: Member ID %d does not exist.\n", borrowing.member_id);
        return 0;
    }
    if (borrowing.borrow_date == 0) {
        printf("Error: Borrow date cannot be empty.\n");
        return 0;
    }
    FILE* file = fopen("database/borrowings.dat", "ab");
    if (!file) return 0;

    borrowing.borrowing_id = getNextId("database/borrowings.dat");
    fwrite(&borrowing, sizeof(Borrowing), 1, file);
    fclose(file);
    return 1;
}

int updateBorrowing(Borrowing borrowing) {
    // Referential integrity: check book and member exist
    if (borrowing.book_id <= 0 || getBook(borrowing.book_id) == NULL) {
        printf("Error: Book ID %d does not exist.\n", borrowing.book_id);
        return 0;
    }
    if (borrowing.member_id <= 0 || getMember(borrowing.member_id) == NULL) {
        printf("Error: Member ID %d does not exist.\n", borrowing.member_id);
        return 0;
    }
    if (borrowing.borrow_date == 0) {
        printf("Error: Borrow date cannot be empty.\n");
        return 0;
    }
    FILE* file = fopen("database/borrowings.dat", "rb+");
    if (!file) return 0;

    Borrowing temp;
    while (fread(&temp, sizeof(Borrowing), 1, file) == 1) {
        if (temp.borrowing_id == borrowing.borrowing_id) {
            long long offset = -(long long)sizeof(Borrowing);
            fseek(file, offset, SEEK_CUR);
            fwrite(&borrowing, sizeof(Borrowing), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deleteBorrowing(int borrowing_id) {
    // Check referential integrity: is this borrowing referenced by any fine?
    int fine_count = 0;
    Fine* fines = getAllFines(&fine_count);
    if (fines) {
        for (int i = 0; i < fine_count; i++) {
            if (fines[i].borrowing_id == borrowing_id) {
                printf("Error: Cannot delete borrowing. There are fines referencing this borrowing.\n");
                free(fines);
                return 0;
            }
        }
        free(fines);
    }
    FILE* file = fopen("database/borrowings.dat", "rb");
    if (!file) return 0;

    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }

    Borrowing borrowing;
    int found = 0;
    while (fread(&borrowing, sizeof(Borrowing), 1, file) == 1) {
        if (borrowing.borrowing_id != borrowing_id) {
            fwrite(&borrowing, sizeof(Borrowing), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("database/borrowings.dat");
    rename("database/temp.dat", "database/borrowings.dat");

    return found;
}

Borrowing* getBorrowing(int borrowing_id) {
    FILE* file = fopen("database/borrowings.dat", "rb");
    if (!file) return NULL;

    Borrowing* borrowing = (Borrowing*)malloc(sizeof(Borrowing));
    while (fread(borrowing, sizeof(Borrowing), 1, file) == 1) {
        if (borrowing->borrowing_id == borrowing_id) {
            fclose(file);
            return borrowing;
        }
    }

    fclose(file);
    free(borrowing);
    return NULL;
}

Borrowing* getAllBorrowings(int* count) {
    FILE* file = fopen("database/borrowings.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Borrowing);
    fseek(file, 0, SEEK_SET);

    Borrowing* borrowings = (Borrowing*)malloc(*count * sizeof(Borrowing));
    fread(borrowings, sizeof(Borrowing), *count, file);
    fclose(file);

    return borrowings;
}

// Fine operations
int addFine(Fine fine) {
    // Referential integrity: check borrowing exists
    if (fine.borrowing_id <= 0 || getBorrowing(fine.borrowing_id) == NULL) {
        printf("Error: Borrowing ID %d does not exist.\n", fine.borrowing_id);
        return 0;
    }
    if (fine.amount < 0) {
        printf("Error: Fine amount cannot be negative.\n");
        return 0;
    }
    FILE* file = fopen("database/fines.dat", "ab");
    if (!file) return 0;

    fine.fine_id = getNextId("database/fines.dat");
    fwrite(&fine, sizeof(Fine), 1, file);
    fclose(file);
    return 1;
}

int updateFine(Fine fine) {
    // Referential integrity: check borrowing exists
    if (fine.borrowing_id <= 0 || getBorrowing(fine.borrowing_id) == NULL) {
        printf("Error: Borrowing ID %d does not exist.\n", fine.borrowing_id);
        return 0;
    }
    if (fine.amount < 0) {
        printf("Error: Fine amount cannot be negative.\n");
        return 0;
    }
    FILE* file = fopen("database/fines.dat", "rb+");
    if (!file) return 0;

    Fine temp;
    while (fread(&temp, sizeof(Fine), 1, file) == 1) {
        if (temp.fine_id == fine.fine_id) {
            long long offset = -(long long)sizeof(Fine);
            fseek(file, offset, SEEK_CUR);
            fwrite(&fine, sizeof(Fine), 1, file);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int deleteFine(int fine_id) {
    FILE* file = fopen("database/fines.dat", "rb");
    if (!file) return 0;

    FILE* tempFile = fopen("database/temp.dat", "wb");
    if (!tempFile) {
        fclose(file);
        return 0;
    }

    Fine fine;
    int found = 0;
    while (fread(&fine, sizeof(Fine), 1, file) == 1) {
        if (fine.fine_id != fine_id) {
            fwrite(&fine, sizeof(Fine), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("database/fines.dat");
    rename("database/temp.dat", "database/fines.dat");

    return found;
}

Fine* getFine(int fine_id) {
    FILE* file = fopen("database/fines.dat", "rb");
    if (!file) return NULL;

    Fine* fine = (Fine*)malloc(sizeof(Fine));
    while (fread(fine, sizeof(Fine), 1, file) == 1) {
        if (fine->fine_id == fine_id) {
            fclose(file);
            return fine;
        }
    }

    fclose(file);
    free(fine);
    return NULL;
}

Fine* getAllFines(int* count) {
    FILE* file = fopen("database/fines.dat", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Fine);
    fseek(file, 0, SEEK_SET);

    Fine* fines = (Fine*)malloc(*count * sizeof(Fine));
    fread(fines, sizeof(Fine), *count, file);
    fclose(file);

    return fines;
} 