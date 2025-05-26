#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "../../include/database.h"
#include "../../include/book.h"
#include "../../include/author.h"
#include "../../include/publisher.h"
#include "../../include/member.h"
#include "../../include/staff.h"
#include "../../include/borrowing.h"
#include "../../include/fine.h"

// Color definitions
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD "\033[1m"

// Global variables
static Database *db;
static HANDLE hConsole;

// Function prototypes
static void clear_screen(void);
static void print_header(const char *title);
static void print_menu(void);
static void print_error(const char *message);
static void print_success(const char *message);
static void print_info(const char *message);
static void handle_books(void);
static void handle_authors(void);
static void handle_publishers(void);
static void handle_members(void);
static void handle_staff(void);
static void handle_borrowings(void);
static void handle_fines(void);

// Initialize console
void init_console(void) {
    // Get console handle
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Enable ANSI escape sequences
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    // Set console title
    SetConsoleTitle("Library Management System");
    
    // Set console size
    SMALL_RECT windowSize = {0, 0, 100, 30};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

// Clear screen
static void clear_screen(void) {
    system("cls");
}

// Print header
static void print_header(const char *title) {
    printf("\n%s%s%s\n", COLOR_CYAN, title, COLOR_RESET);
    printf("%s%s%s\n", COLOR_CYAN, "==========================================", COLOR_RESET);
}

// Print menu
static void print_menu(void) {
    clear_screen();
    print_header("Library Management System");
    printf("\n%sMain Menu:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s1.%s Books\n", COLOR_GREEN, COLOR_RESET);
    printf("%s2.%s Authors\n", COLOR_GREEN, COLOR_RESET);
    printf("%s3.%s Publishers\n", COLOR_GREEN, COLOR_RESET);
    printf("%s4.%s Members\n", COLOR_GREEN, COLOR_RESET);
    printf("%s5.%s Staff\n", COLOR_GREEN, COLOR_RESET);
    printf("%s6.%s Borrowings\n", COLOR_GREEN, COLOR_RESET);
    printf("%s7.%s Fines\n", COLOR_GREEN, COLOR_RESET);
    printf("%s0.%s Exit\n", COLOR_RED, COLOR_RESET);
    printf("\nEnter your choice: ");
}

// Print error message
static void print_error(const char *message) {
    printf("%sError: %s%s\n", COLOR_RED, message, COLOR_RESET);
}

// Print success message
static void print_success(const char *message) {
    printf("%sSuccess: %s%s\n", COLOR_GREEN, message, COLOR_RESET);
}

// Print info message
static void print_info(const char *message) {
    printf("%s%s%s\n", COLOR_YELLOW, message, COLOR_RESET);
}

// Main console loop
void run_console(Database *database) {
    db = database;
    init_console();
    
    int choice;
    char input[10];
    
    while (1) {
        print_menu();
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);
        
        switch (choice) {
            case 1:
                handle_books();
                break;
            case 2:
                handle_authors();
                break;
            case 3:
                handle_publishers();
                break;
            case 4:
                handle_members();
                break;
            case 5:
                handle_staff();
                break;
            case 6:
                handle_borrowings();
                break;
            case 7:
                handle_fines();
                break;
            case 0:
                print_info("Thank you for using Library Management System!");
                return;
            default:
                print_error("Invalid choice. Please try again.");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// Handle books menu
static void handle_books(void) {
    int choice;
    char input[10];
    
    while (1) {
        clear_screen();
        print_header("Books Management");
        printf("\n%s1.%s List all books\n", COLOR_GREEN, COLOR_RESET);
        printf("%s2.%s Add new book\n", COLOR_GREEN, COLOR_RESET);
        printf("%s3.%s Find book by ID\n", COLOR_GREEN, COLOR_RESET);
        printf("%s4.%s Update book\n", COLOR_GREEN, COLOR_RESET);
        printf("%s5.%s Delete book\n", COLOR_GREEN, COLOR_RESET);
        printf("%s0.%s Back to main menu\n", COLOR_RED, COLOR_RESET);
        printf("\nEnter your choice: ");
        
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);
        
        switch (choice) {
            case 1: {
                BookList *books = book_list_all(db);
                if (books != NULL) {
                    printf("\n%s%-5s %-30s %-15s %-10s%s\n", COLOR_BOLD, "ID", "Title", "ISBN", "Category", COLOR_RESET);
                    printf("%s%s%s\n", COLOR_CYAN, "------------------------------------------------------------", COLOR_RESET);
                    for (int i = 0; i < books->count; i++) {
                        printf("%-5d %-30s %-15s %-10s\n",
                               books->books[i]->book_id,
                               books->books[i]->title,
                               books->books[i]->isbn,
                               books->books[i]->category);
                    }
                    book_list_free(books);
                } else {
                    print_error("Failed to fetch books");
                }
                break;
            }
            case 2: {
                char title[100], isbn[20], category[50];
                int author_id, publisher_id, year;
                double price;
                
                printf("\nEnter book details:\n");
                printf("Title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                
                printf("ISBN: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strcspn(isbn, "\n")] = 0;
                
                printf("Category: ");
                fgets(category, sizeof(category), stdin);
                category[strcspn(category, "\n")] = 0;
                
                printf("Author ID: ");
                fgets(input, sizeof(input), stdin);
                author_id = atoi(input);
                
                printf("Publisher ID: ");
                fgets(input, sizeof(input), stdin);
                publisher_id = atoi(input);
                
                printf("Publication Year: ");
                fgets(input, sizeof(input), stdin);
                year = atoi(input);
                
                printf("Price: ");
                fgets(input, sizeof(input), stdin);
                price = atof(input);
                
                Book *book = book_create(title, isbn, category, author_id, publisher_id, year, price);
                if (book != NULL) {
                    if (book_save(db, book)) {
                        print_success("Book added successfully");
                    } else {
                        print_error("Failed to add book");
                    }
                    book_free(book);
                } else {
                    print_error("Failed to create book");
                }
                break;
            }
            case 3: {
                int id;
                printf("\nEnter book ID: ");
                fgets(input, sizeof(input), stdin);
                id = atoi(input);
                
                Book *book = book_find_by_id(db, id);
                if (book != NULL) {
                    printf("\n%sBook Details:%s\n", COLOR_BOLD, COLOR_RESET);
                    printf("ID: %d\n", book->book_id);
                    printf("Title: %s\n", book->title);
                    printf("ISBN: %s\n", book->isbn);
                    printf("Category: %s\n", book->category);
                    printf("Author ID: %d\n", book->author_id);
                    printf("Publisher ID: %d\n", book->publisher_id);
                    printf("Publication Year: %d\n", book->publication_year);
                    printf("Price: %.2f\n", book->price);
                    book_free(book);
                } else {
                    print_error("Book not found");
                }
                break;
            }
            case 4: {
                int id;
                printf("\nEnter book ID to update: ");
                fgets(input, sizeof(input), stdin);
                id = atoi(input);
                
                Book *book = book_find_by_id(db, id);
                if (book != NULL) {
                    char title[100], isbn[20], category[50];
                    int author_id, publisher_id, year;
                    double price;
                    
                    printf("\nEnter new details (press Enter to keep current value):\n");
                    
                    printf("Title [%s]: ", book->title);
                    fgets(title, sizeof(title), stdin);
                    title[strcspn(title, "\n")] = 0;
                    if (strlen(title) > 0) {
                        strncpy(book->title, title, sizeof(book->title) - 1);
                    }
                    
                    printf("ISBN [%s]: ", book->isbn);
                    fgets(isbn, sizeof(isbn), stdin);
                    isbn[strcspn(isbn, "\n")] = 0;
                    if (strlen(isbn) > 0) {
                        strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
                    }
                    
                    printf("Category [%s]: ", book->category);
                    fgets(category, sizeof(category), stdin);
                    category[strcspn(category, "\n")] = 0;
                    if (strlen(category) > 0) {
                        strncpy(book->category, category, sizeof(book->category) - 1);
                    }
                    
                    printf("Author ID [%d]: ", book->author_id);
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) {
                        book->author_id = atoi(input);
                    }
                    
                    printf("Publisher ID [%d]: ", book->publisher_id);
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) {
                        book->publisher_id = atoi(input);
                    }
                    
                    printf("Publication Year [%d]: ", book->publication_year);
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) {
                        book->publication_year = atoi(input);
                    }
                    
                    printf("Price [%.2f]: ", book->price);
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) {
                        book->price = atof(input);
                    }
                    
                    if (book_update(db, book)) {
                        print_success("Book updated successfully");
                    } else {
                        print_error("Failed to update book");
                    }
                    book_free(book);
                } else {
                    print_error("Book not found");
                }
                break;
            }
            case 5: {
                int id;
                printf("\nEnter book ID to delete: ");
                fgets(input, sizeof(input), stdin);
                id = atoi(input);
                
                if (book_delete(db, id)) {
                    print_success("Book deleted successfully");
                } else {
                    print_error("Failed to delete book");
                }
                break;
            }
            case 0:
                return;
            default:
                print_error("Invalid choice. Please try again.");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// Similar implementations for handle_authors(), handle_publishers(), handle_members(),
// handle_staff(), handle_borrowings(), and handle_fines() following the same pattern
// as handle_books() but with their respective fields and operations. 