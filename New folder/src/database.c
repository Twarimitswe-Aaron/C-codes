#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/database.h"

#define MAX_RECORDS 1000

// File paths for .dat files
const char* BOOKS_FILE = "../database/books.dat";
const char* AUTHORS_FILE = "../database/authors.dat";
const char* PUBLISHERS_FILE = "../database/publishers.dat";
const char* MEMBERS_FILE = "../database/members.dat";
const char* STAFF_FILE = "../database/staff.dat";
const char* BORROWINGS_FILE = "../database/borrowings.dat";
const char* FINES_FILE = "../database/fines.dat";

// Initialize database files
void init_database() {
    FILE* files[] = {
        fopen(BOOKS_FILE, "ab+"),
        fopen(AUTHORS_FILE, "ab+"),
        fopen(PUBLISHERS_FILE, "ab+"),
        fopen(MEMBERS_FILE, "ab+"),
        fopen(STAFF_FILE, "ab+"),
        fopen(BORROWINGS_FILE, "ab+"),
        fopen(FINES_FILE, "ab+")
    };

    for (int i = 0; i < 7; i++) {
        if (files[i] == NULL) {
            printf("Error opening database file %d\n", i);
            exit(1);
        }
        fclose(files[i]);
    }
}

// Close database files
void close_database() {
    // Files are closed after each operation, so no need for explicit closing
}

// Book operations
int add_book(Book book) {
    FILE* file = fopen(BOOKS_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&book, sizeof(Book), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_book(Book book) {
    FILE* file = fopen(BOOKS_FILE, "rb+");
    if (file == NULL) return -1;

    Book temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Book), 1, file) == 1) {
        if (temp.book_id == book.book_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&book, sizeof(Book), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

int delete_book(int book_id) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (file == NULL) return -1;

    FILE* temp_file = fopen("temp.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return -1;
    }

    Book book;
    int found = 0;

    while (fread(&book, sizeof(Book), 1, file) == 1) {
        if (book.book_id != book_id) {
            fwrite(&book, sizeof(Book), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(BOOKS_FILE);
    rename("temp.dat", BOOKS_FILE);

    return found ? 0 : -1;
}

Book* get_book(int book_id) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (file == NULL) return NULL;

    Book* book = malloc(sizeof(Book));
    if (book == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(book, sizeof(Book), 1, file) == 1) {
        if (book->book_id == book_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? book : NULL;
}

Book* get_all_books(int* count) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (file == NULL) return NULL;

    Book* books = malloc(MAX_RECORDS * sizeof(Book));
    if (books == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&books[*count], sizeof(Book), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return books;
}

// Author operations
int add_author(Author author) {
    FILE* file = fopen(AUTHORS_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&author, sizeof(Author), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_author(Author author) {
    FILE* file = fopen(AUTHORS_FILE, "rb+");
    if (file == NULL) return -1;

    Author temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Author), 1, file) == 1) {
        if (temp.author_id == author.author_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&author, sizeof(Author), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

int delete_author(int author_id) {
    FILE* file = fopen(AUTHORS_FILE, "rb");
    if (file == NULL) return -1;

    FILE* temp_file = fopen("temp.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return -1;
    }

    Author author;
    int found = 0;

    while (fread(&author, sizeof(Author), 1, file) == 1) {
        if (author.author_id != author_id) {
            fwrite(&author, sizeof(Author), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(AUTHORS_FILE);
    rename("temp.dat", AUTHORS_FILE);

    return found ? 0 : -1;
}

Author* get_author(int author_id) {
    FILE* file = fopen(AUTHORS_FILE, "rb");
    if (file == NULL) return NULL;

    Author* author = malloc(sizeof(Author));
    if (author == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(author, sizeof(Author), 1, file) == 1) {
        if (author->author_id == author_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? author : NULL;
}

Author* get_all_authors(int* count) {
    FILE* file = fopen(AUTHORS_FILE, "rb");
    if (file == NULL) return NULL;

    Author* authors = malloc(MAX_RECORDS * sizeof(Author));
    if (authors == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&authors[*count], sizeof(Author), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return authors;
}

// Publisher operations
int add_publisher(Publisher publisher) {
    FILE* file = fopen(PUBLISHERS_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&publisher, sizeof(Publisher), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_publisher(Publisher publisher) {
    FILE* file = fopen(PUBLISHERS_FILE, "rb+");
    if (file == NULL) return -1;

    Publisher temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Publisher), 1, file) == 1) {
        if (temp.publisher_id == publisher.publisher_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&publisher, sizeof(Publisher), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

int delete_publisher(int publisher_id) {
    FILE* file = fopen(PUBLISHERS_FILE, "rb");
    if (file == NULL) return -1;

    FILE* temp_file = fopen("temp.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return -1;
    }

    Publisher publisher;
    int found = 0;

    while (fread(&publisher, sizeof(Publisher), 1, file) == 1) {
        if (publisher.publisher_id != publisher_id) {
            fwrite(&publisher, sizeof(Publisher), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(PUBLISHERS_FILE);
    rename("temp.dat", PUBLISHERS_FILE);

    return found ? 0 : -1;
}

Publisher* get_publisher(int publisher_id) {
    FILE* file = fopen(PUBLISHERS_FILE, "rb");
    if (file == NULL) return NULL;

    Publisher* publisher = malloc(sizeof(Publisher));
    if (publisher == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(publisher, sizeof(Publisher), 1, file) == 1) {
        if (publisher->publisher_id == publisher_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? publisher : NULL;
}

Publisher* get_all_publishers(int* count) {
    FILE* file = fopen(PUBLISHERS_FILE, "rb");
    if (file == NULL) return NULL;

    Publisher* publishers = malloc(MAX_RECORDS * sizeof(Publisher));
    if (publishers == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&publishers[*count], sizeof(Publisher), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return publishers;
}

// Member operations
int add_member(Member member) {
    FILE* file = fopen(MEMBERS_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&member, sizeof(Member), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_member(Member member) {
    FILE* file = fopen(MEMBERS_FILE, "rb+");
    if (file == NULL) return -1;

    Member temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Member), 1, file) == 1) {
        if (temp.member_id == member.member_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&member, sizeof(Member), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

int delete_member(int member_id) {
    FILE* file = fopen(MEMBERS_FILE, "rb");
    if (file == NULL) return -1;

    FILE* temp_file = fopen("temp.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return -1;
    }

    Member member;
    int found = 0;

    while (fread(&member, sizeof(Member), 1, file) == 1) {
        if (member.member_id != member_id) {
            fwrite(&member, sizeof(Member), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(MEMBERS_FILE);
    rename("temp.dat", MEMBERS_FILE);

    return found ? 0 : -1;
}

Member* get_member(int member_id) {
    FILE* file = fopen(MEMBERS_FILE, "rb");
    if (file == NULL) return NULL;

    Member* member = malloc(sizeof(Member));
    if (member == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(member, sizeof(Member), 1, file) == 1) {
        if (member->member_id == member_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? member : NULL;
}

Member* get_all_members(int* count) {
    FILE* file = fopen(MEMBERS_FILE, "rb");
    if (file == NULL) return NULL;

    Member* members = malloc(MAX_RECORDS * sizeof(Member));
    if (members == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&members[*count], sizeof(Member), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return members;
}

// Staff operations
int add_staff(Staff staff) {
    FILE* file = fopen(STAFF_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&staff, sizeof(Staff), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_staff(Staff staff) {
    FILE* file = fopen(STAFF_FILE, "rb+");
    if (file == NULL) return -1;

    Staff temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Staff), 1, file) == 1) {
        if (temp.staff_id == staff.staff_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&staff, sizeof(Staff), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

int delete_staff(int staff_id) {
    FILE* file = fopen(STAFF_FILE, "rb");
    if (file == NULL) return -1;

    FILE* temp_file = fopen("temp.dat", "wb");
    if (temp_file == NULL) {
        fclose(file);
        return -1;
    }

    Staff staff;
    int found = 0;

    while (fread(&staff, sizeof(Staff), 1, file) == 1) {
        if (staff.staff_id != staff_id) {
            fwrite(&staff, sizeof(Staff), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(STAFF_FILE);
    rename("temp.dat", STAFF_FILE);

    return found ? 0 : -1;
}

Staff* get_staff(int staff_id) {
    FILE* file = fopen(STAFF_FILE, "rb");
    if (file == NULL) return NULL;

    Staff* staff = malloc(sizeof(Staff));
    if (staff == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(staff, sizeof(Staff), 1, file) == 1) {
        if (staff->staff_id == staff_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? staff : NULL;
}

Staff* get_all_staff(int* count) {
    FILE* file = fopen(STAFF_FILE, "rb");
    if (file == NULL) return NULL;

    Staff* staff_list = malloc(MAX_RECORDS * sizeof(Staff));
    if (staff_list == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&staff_list[*count], sizeof(Staff), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return staff_list;
}

// Borrowing operations
int add_borrowing(Borrowing borrowing) {
    FILE* file = fopen(BORROWINGS_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&borrowing, sizeof(Borrowing), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_borrowing(Borrowing borrowing) {
    FILE* file = fopen(BORROWINGS_FILE, "rb+");
    if (file == NULL) return -1;

    Borrowing temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Borrowing), 1, file) == 1) {
        if (temp.borrowing_id == borrowing.borrowing_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&borrowing, sizeof(Borrowing), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

Borrowing* get_borrowing(int borrowing_id) {
    FILE* file = fopen(BORROWINGS_FILE, "rb");
    if (file == NULL) return NULL;

    Borrowing* borrowing = malloc(sizeof(Borrowing));
    if (borrowing == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(borrowing, sizeof(Borrowing), 1, file) == 1) {
        if (borrowing->borrowing_id == borrowing_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? borrowing : NULL;
}

Borrowing* get_all_borrowings(int* count) {
    FILE* file = fopen(BORROWINGS_FILE, "rb");
    if (file == NULL) return NULL;

    Borrowing* borrowings = malloc(MAX_RECORDS * sizeof(Borrowing));
    if (borrowings == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&borrowings[*count], sizeof(Borrowing), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return borrowings;
}

// Fine operations
int add_fine(Fine fine) {
    FILE* file = fopen(FINES_FILE, "ab");
    if (file == NULL) return -1;

    int result = fwrite(&fine, sizeof(Fine), 1, file);
    fclose(file);
    return result == 1 ? 0 : -1;
}

int update_fine(Fine fine) {
    FILE* file = fopen(FINES_FILE, "rb+");
    if (file == NULL) return -1;

    Fine temp;
    int found = 0;
    long pos = 0;

    while (fread(&temp, sizeof(Fine), 1, file) == 1) {
        if (temp.fine_id == fine.fine_id) {
            fseek(file, pos, SEEK_SET);
            fwrite(&fine, sizeof(Fine), 1, file);
            found = 1;
            break;
        }
        pos = ftell(file);
    }

    fclose(file);
    return found ? 0 : -1;
}

Fine* get_fine(int fine_id) {
    FILE* file = fopen(FINES_FILE, "rb");
    if (file == NULL) return NULL;

    Fine* fine = malloc(sizeof(Fine));
    if (fine == NULL) {
        fclose(file);
        return NULL;
    }

    int found = 0;
    while (fread(fine, sizeof(Fine), 1, file) == 1) {
        if (fine->fine_id == fine_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? fine : NULL;
}

Fine* get_all_fines(int* count) {
    FILE* file = fopen(FINES_FILE, "rb");
    if (file == NULL) return NULL;

    Fine* fines = malloc(MAX_RECORDS * sizeof(Fine));
    if (fines == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fread(&fines[*count], sizeof(Fine), 1, file) == 1 && *count < MAX_RECORDS) {
        (*count)++;
    }

    fclose(file);
    return fines;
} 