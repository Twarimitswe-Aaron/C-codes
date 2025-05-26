#include "database.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File paths for storing data
#define AUTHORS_FILE "data/authors.txt"
#define PUBLISHERS_FILE "data/publishers.txt"
#define STAFF_FILE "data/staff.txt"
#define FINES_FILE "data/fines.txt"

// Helper function to ensure data directory exists
static void ensure_data_directory() {
    // Create data directory if it doesn't exist
    #ifdef _WIN32
    system("if not exist data mkdir data");
    #else
    system("mkdir -p data");
    #endif
}

// Helper function to get next available ID
static int get_next_id(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return 1;

    int max_id = 0;
    void* buffer = malloc(1024); // Temporary buffer
    size_t size;

    while ((size = fread(buffer, 1, 1024, file)) > 0) {
        // Process buffer to find max ID
        // This is a simplified version - in reality, you'd need to parse the actual structure
        for (size_t i = 0; i < size; i++) {
            if (((char*)buffer)[i] > max_id) {
                max_id = ((char*)buffer)[i];
            }
        }
    }

    free(buffer);
    fclose(file);
    return max_id + 1;
}

// Initialize the database by creating necessary files
void init_database() {
    ensure_data_directory();
    
    // Create empty files if they don't exist
    FILE* files[] = {
        fopen(BOOKS_FILE, "a"),
        fopen(AUTHORS_FILE, "a"),
        fopen(PUBLISHERS_FILE, "a"),
        fopen(MEMBERS_FILE, "a"),
        fopen(STAFF_FILE, "a"),
        fopen(BORROWINGS_FILE, "a"),
        fopen(FINES_FILE, "a")
    };
    
    // Close all files
    for (int i = 0; i < 7; i++) {
        if (files[i]) fclose(files[i]);
    }
}

// Initialize the data directory and files
int init_files() {
    // Create data directory if it doesn't exist
    #ifdef _WIN32
    system("if not exist data mkdir data");
    #else
    system("mkdir -p data");
    #endif

    // Create empty files if they don't exist
    FILE* file;
    
    file = fopen(BOOKS_FILE, "a");
    if (file) fclose(file);
    
    file = fopen(MEMBERS_FILE, "a");
    if (file) fclose(file);
    
    file = fopen(BORROWINGS_FILE, "a");
    if (file) fclose(file);
    
    return 1;
}

// Add a new book to the books file
int add_book(Book book) {
    FILE* file = fopen(BOOKS_FILE, "a");
    if (!file) return 0;
    
    fprintf(file, "%d|%s|%s|%s|%d\n",
            book.book_id,
            book.title,
            book.author,
            book.isbn,
            book.copies_available);
    
    fclose(file);
    return 1;
}

// Add a new member to the members file
int add_member(Member member) {
    FILE* file = fopen(MEMBERS_FILE, "a");
    if (!file) return 0;
    
    fprintf(file, "%d|%s|%s|%s\n",
            member.member_id,
            member.name,
            member.phone,
            member.email);
    
    fclose(file);
    return 1;
}

// Add a new borrowing to the borrowings file
int add_borrowing(Borrowing borrowing) {
    FILE* file = fopen(BORROWINGS_FILE, "a");
    if (!file) return 0;
    
    fprintf(file, "%d|%d|%d|%s|%s|%s\n",
            borrowing.borrowing_id,
            borrowing.book_id,
            borrowing.member_id,
            borrowing.borrow_date,
            borrowing.due_date,
            borrowing.return_date);
    
    fclose(file);
    return 1;
}

// Display all books
void display_all_books() {
    FILE* file = fopen(BOOKS_FILE, "r");
    if (!file) {
        printf("No books found.\n");
        return;
    }
    
    printf("\n=== Books ===\n");
    printf("ID | Title | Author | ISBN | Available Copies\n");
    printf("----------------------------------------\n");
    
    char line[MAX_TEXT_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int id, copies;
        char title[MAX_STRING_LENGTH], author[MAX_STRING_LENGTH], isbn[MAX_STRING_LENGTH];
        
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%d",
               &id, title, author, isbn, &copies);
        
        printf("%d | %s | %s | %s | %d\n",
               id, title, author, isbn, copies);
    }
    
    fclose(file);
}

// Display all members
void display_all_members() {
    FILE* file = fopen(MEMBERS_FILE, "r");
    if (!file) {
        printf("No members found.\n");
        return;
    }
    
    printf("\n=== Members ===\n");
    printf("ID | Name | Phone | Email\n");
    printf("----------------------------------------\n");
    
    char line[MAX_TEXT_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[MAX_STRING_LENGTH], phone[MAX_STRING_LENGTH], email[MAX_STRING_LENGTH];
        
        sscanf(line, "%d|%[^|]|%[^|]|%[^\n]",
               &id, name, phone, email);
        
        printf("%d | %s | %s | %s\n",
               id, name, phone, email);
    }
    
    fclose(file);
}

// Display all borrowings
void display_all_borrowings() {
    FILE* file = fopen(BORROWINGS_FILE, "r");
    if (!file) {
        printf("No borrowings found.\n");
        return;
    }
    
    printf("\n=== Borrowings ===\n");
    printf("ID | Book ID | Member ID | Borrow Date | Due Date | Return Date\n");
    printf("--------------------------------------------------------\n");
    
    char line[MAX_TEXT_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int id, book_id, member_id;
        char borrow_date[11], due_date[11], return_date[11];
        
        sscanf(line, "%d|%d|%d|%[^|]|%[^|]|%[^\n]",
               &id, &book_id, &member_id, borrow_date, due_date, return_date);
        
        printf("%d | %d | %d | %s | %s | %s\n",
               id, book_id, member_id, borrow_date, due_date, return_date);
    }
    
    fclose(file);
}

// Book operations implementation
int update_book(Book book) {
    FILE* file = fopen(BOOKS_FILE, "rb+");
    if (!file) return 0;
    
    Book temp;
    int found = 0;
    
    // Search for the book and update it
    while (fread(&temp, sizeof(Book), 1, file)) {
        if (temp.book_id == book.book_id) {
            fseek(file, -(long long)sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            found = 1;
            break;
        }
    }
    
    fclose(file);
    return found;
}

int delete_book(int book_id) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (!file) return 0;
    
    // Create temporary file
    FILE* temp = fopen("data/temp.dat", "wb");
    if (!temp) {
        fclose(file);
        return 0;
    }
    
    Book book;
    int found = 0;
    
    // Copy all books except the one to delete
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.book_id != book_id) {
            fwrite(&book, sizeof(Book), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(file);
    fclose(temp);
    
    // Replace original file with temporary file
    remove(BOOKS_FILE);
    rename("data/temp.dat", BOOKS_FILE);
    
    return found;
}

Book* get_book(int book_id) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (!file) return NULL;
    
    Book* book = malloc(sizeof(Book));
    if (!book) {
        fclose(file);
        return NULL;
    }
    
    int found = 0;
    while (fread(book, sizeof(Book), 1, file)) {
        if (book->book_id == book_id) {
            found = 1;
            break;
        }
    }
    
    fclose(file);
    if (!found) {
        free(book);
        return NULL;
    }
    
    return book;
}

Book* get_all_books(int* count) {
    FILE* file = fopen(BOOKS_FILE, "rb");
    if (!file) return NULL;
    
    // Count total books
    fseek(file, 0, SEEK_END);
    *count = ftell(file) / sizeof(Book);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory for all books
    Book* books = malloc(*count * sizeof(Book));
    if (!books) {
        fclose(file);
        return NULL;
    }
    
    // Read all books
    fread(books, sizeof(Book), *count, file);
    fclose(file);
    
    return books;
}

// Author operations
int add_author(Author author) {
    FILE* file = fopen(AUTHORS_FILE, "ab");
    if (!file) return 0;
    
    author.author_id = get_next_id(AUTHORS_FILE);
    int result = fwrite(&author, sizeof(Author), 1, file);
    fclose(file);
    return result;
}

Author* get_author(int author_id) {
    FILE* file = fopen(AUTHORS_FILE, "rb");
    if (!file) return NULL;

    Author* author = malloc(sizeof(Author));
    int found = 0;

    while (fread(author, sizeof(Author), 1, file)) {
        if (author->author_id == author_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? author : NULL;
}

// Publisher operations
Publisher* get_publisher(int publisher_id) {
    FILE* file = fopen(PUBLISHERS_FILE, "rb");
    if (!file) return NULL;

    Publisher* publisher = malloc(sizeof(Publisher));
    int found = 0;

    while (fread(publisher, sizeof(Publisher), 1, file)) {
        if (publisher->publisher_id == publisher_id) {
            found = 1;
            break;
        }
    }

    fclose(file);
    return found ? publisher : NULL;
}

// Staff operations
int add_staff(Staff staff) {
    FILE* file = fopen(STAFF_FILE, "ab");
    if (!file) return 0;
    
    int result = fwrite(&staff, sizeof(Staff), 1, file);
    fclose(file);
    return result;
}

// Fine operations
int add_fine(Fine fine) {
    FILE* file = fopen(FINES_FILE, "ab");
    if (!file) return 0;
    
    int result = fwrite(&fine, sizeof(Fine), 1, file);
    fclose(file);
    return result;
}

// Stub implementations for missing functions
Member* get_member(int member_id) { return NULL; }
Borrowing* get_borrowing(int borrowing_id) { return NULL; }
int update_borrowing(Borrowing b) { return 0; }
int delete_borrowing(int borrowing_id) { return 0; }
Borrowing* get_all_borrowings(int* count) { *count = 0; return NULL; }
int update_member(Member m) { return 0; }
int delete_member(int member_id) { return 0; }
Member* get_all_members(int* count) { *count = 0; return NULL; } 