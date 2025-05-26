#include "database.h"
#include <stdio.h>
#include <string.h>

// Function declarations
void run_console_ui();

// Print usage information
void print_usage(const char* program_name) {
    printf("Usage: %s [--console|--gui]\n", program_name);
    printf("Options:\n");
    printf("  --console    Run in console mode\n");
    printf("  --gui        Run in GUI mode\n");
    printf("If no option is provided, console mode will be used by default.\n");
}

void run_console_ui() {
    int choice;
    while (1) {
        printf("\n=== Library Management System ===\n");
        printf("1. Display all books\n");
        printf("2. Display all members\n");
        printf("3. Display all borrowings\n");
        printf("4. Add new book\n");
        printf("5. Add new member\n");
        printf("6. Add new borrowing\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 0:
                return;
            case 1:
                display_all_books();
                break;
            case 2:
                display_all_members();
                break;
            case 3:
                display_all_borrowings();
                break;
            case 4: {
                Book book;
                printf("Enter book details:\n");
                printf("ID: ");
                scanf("%d", &book.book_id);
                printf("Title: ");
                scanf(" %[^\n]s", book.title);
                printf("Author: ");
                scanf(" %[^\n]s", book.author);
                printf("ISBN: ");
                scanf(" %[^\n]s", book.isbn);
                printf("Available copies: ");
                scanf("%d", &book.copies_available);
                
                if (add_book(book)) {
                    printf("Book added successfully!\n");
                } else {
                    printf("Failed to add book.\n");
                }
                break;
            }
            case 5: {
                Member member;
                printf("Enter member details:\n");
                printf("ID: ");
                scanf("%d", &member.member_id);
                printf("Name: ");
                scanf(" %[^\n]s", member.name);
                printf("Phone: ");
                scanf(" %[^\n]s", member.phone);
                printf("Email: ");
                scanf(" %[^\n]s", member.email);
                
                if (add_member(member)) {
                    printf("Member added successfully!\n");
                } else {
                    printf("Failed to add member.\n");
                }
                break;
            }
            case 6: {
                Borrowing borrowing;
                printf("Enter borrowing details:\n");
                printf("ID: ");
                scanf("%d", &borrowing.borrowing_id);
                printf("Book ID: ");
                scanf("%d", &borrowing.book_id);
                printf("Member ID: ");
                scanf("%d", &borrowing.member_id);
                printf("Borrow date (YYYY-MM-DD): ");
                scanf(" %[^\n]s", borrowing.borrow_date);
                printf("Due date (YYYY-MM-DD): ");
                scanf(" %[^\n]s", borrowing.due_date);
                printf("Return date (YYYY-MM-DD): ");
                scanf(" %[^\n]s", borrowing.return_date);
                
                if (add_borrowing(borrowing)) {
                    printf("Borrowing added successfully!\n");
                } else {
                    printf("Failed to add borrowing.\n");
                }
                break;
            }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    // Initialize files
    if (!init_files()) {
        printf("Failed to initialize files!\n");
        return 1;
    }
    
    printf("Library Management System initialized successfully!\n");
    run_console_ui();
    
    return 0;
}

