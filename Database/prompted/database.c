#include "database.h"

const char* BOOKS_FILE = "data/books.dat";
const char* AUTHORS_FILE = "data/authors.dat";
const char* PUBLISHERS_FILE = "data/publishers.dat";
const char* MEMBERS_FILE = "data/members.dat";
const char* STAFF_FILE = "data/staff.dat";
const char* BORROWINGS_FILE = "data/borrowings.dat";
const char* FINES_FILE = "data/fines.dat";

void ensureDataDirectory() {
    system("mkdir data 2>nul");
}

void saveBook(Book book) {
    FILE* file = fopen(BOOKS_FILE, "ab");
    if (file) {
        fwrite(&book, sizeof(Book), 1, file);
        fclose(file);
    }
}

void saveAuthor(Author author) {
    FILE* file = fopen(AUTHORS_FILE, "ab");
    if (file) {
        fwrite(&author, sizeof(Author), 1, file);
        fclose(file);
    }
}

void savePublisher(Publisher publisher) {
    FILE* file = fopen(PUBLISHERS_FILE, "ab");
    if (file) {
        fwrite(&publisher, sizeof(Publisher), 1, file);
        fclose(file);
    }
}

void saveMember(Member member) {
    FILE* file = fopen(MEMBERS_FILE, "ab");
    if (file) {
        fwrite(&member, sizeof(Member), 1, file);
        fclose(file);
    }
}

void saveStaff(Staff staff) {
    FILE* file = fopen(STAFF_FILE, "ab");
    if (file) {
        fwrite(&staff, sizeof(Staff), 1, file);
        fclose(file);
    }
}

void saveBorrowing(Borrowing borrowing) {
    FILE* file = fopen(BORROWINGS_FILE, "ab");
    if (file) {
        fwrite(&borrowing, sizeof(Borrowing), 1, file);
        fclose(file);
    }
}

void saveFine(Fine fine) {
    FILE* file = fopen(FINES_FILE, "ab");
    if (file) {
        fwrite(&fine, sizeof(Fine), 1, file);
        fclose(file);
    }
}

Book* loadBooks(int* count) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Book);
    rewind(file);
    Book* books = (Book*)malloc(size);
    if (books) fread(books, sizeof(Book), *count, file);
    fclose(file);
    return books;
}

Author* loadAuthors(int* count) {
    FILE* file = fopen(AUTHORS_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Author);
    rewind(file);
    Author* authors = (Author*)malloc(size);
    if (authors) fread(authors, sizeof(Author), *count, file);
    fclose(file);
    return authors;
}

Publisher* loadPublishers(int* count) {
    FILE* file = fopen(PUBLISHERS_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Publisher);
    rewind(file);
    Publisher* publishers = (Publisher*)malloc(size);
    if (publishers) fread(publishers, sizeof(Publisher), *count, file);
    fclose(file);
    return publishers;
}

Member* loadMembers(int* count) {
    FILE* file = fopen(MEMBERS_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Member);
    rewind(file);
    Member* members = (Member*)malloc(size);
    if (members) fread(members, sizeof(Member), *count, file);
    fclose(file);
    return members;
}

Staff* loadStaff(int* count) {
    FILE* file = fopen(STAFF_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Staff);
    rewind(file);
    Staff* staff = (Staff*)malloc(size);
    if (staff) fread(staff, sizeof(Staff), *count, file);
    fclose(file);
    return staff;
}

Borrowing* loadBorrowings(int* count) {
    FILE* file = fopen(BORROWINGS_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Borrowing);
    rewind(file);
    Borrowing* borrowings = (Borrowing*)malloc(size);
    if (borrowings) fread(borrowings, sizeof(Borrowing), *count, file);
    fclose(file);
    return borrowings;
}

Fine* loadFines(int* count) {
    FILE* file = fopen(FINES_FILE, "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    *count = size / sizeof(Fine);
    rewind(file);
    Fine* fines = (Fine*)malloc(size);
    if (fines) fread(fines, sizeof(Fine), *count, file);
    fclose(file);
    return fines;
}