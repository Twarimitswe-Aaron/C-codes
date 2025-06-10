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
    FILE* file = fopen("database/books.dat", "ab");
    if (!file) return 0;

    book.book_id = getNextId("database/books.dat");
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);
    return 1;
}

int updateBook(Book book) {
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
    FILE* file = fopen("database/authors.dat", "ab");
    if (!file) return 0;

    author.author_id = getNextId("database/authors.dat");
    fwrite(&author, sizeof(Author), 1, file);
    fclose(file);
    return 1;
}

int updateAuthor(Author author) {
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
    FILE* file = fopen("database/publishers.dat", "ab");
    if (!file) return 0;

    publisher.publisher_id = getNextId("database/publishers.dat");
    fwrite(&publisher, sizeof(Publisher), 1, file);
    fclose(file);
    return 1;
}

int updatePublisher(Publisher publisher) {
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
    FILE* file = fopen("database/members.dat", "ab");
    if (!file) return 0;

    member.member_id = getNextId("database/members.dat");
    fwrite(&member, sizeof(Member), 1, file);
    fclose(file);
    return 1;
}

int updateMember(Member member) {
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
    FILE* file = fopen("database/staff.dat", "ab");
    if (!file) return 0;

    staff.staff_id = getNextId("database/staff.dat");
    fwrite(&staff, sizeof(Staff), 1, file);
    fclose(file);
    return 1;
}

int updateStaff(Staff staff) {
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
    FILE* file = fopen("database/borrowings.dat", "ab");
    if (!file) return 0;

    borrowing.borrowing_id = getNextId("database/borrowings.dat");
    fwrite(&borrowing, sizeof(Borrowing), 1, file);
    fclose(file);
    return 1;
}

int updateBorrowing(Borrowing borrowing) {
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
    FILE* file = fopen("database/fines.dat", "ab");
    if (!file) return 0;

    fine.fine_id = getNextId("database/fines.dat");
    fwrite(&fine, sizeof(Fine), 1, file);
    fclose(file);
    return 1;
}

int updateFine(Fine fine) {
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