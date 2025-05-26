#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations for menu handling
void show_main_menu();
void handle_book_menu();
void handle_author_menu();
void handle_publisher_menu();
void handle_member_menu();
void handle_staff_menu();
void handle_borrowing_menu();
void handle_fine_menu();

// Helper function to clear the console screen
void clear_screen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

// Helper function to get user input with a prompt
void get_input(char* buffer, const char* prompt) {
    printf("%s", prompt);
    fgets(buffer, MAX_STRING_LENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
}

// Main menu display and handling
void show_main_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Library Management System ===\n");
        printf("1. Book Management\n");
        printf("2. Member Management\n");
        printf("3. Borrowing Management\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        switch (choice) {
            case 1: handle_book_menu(); break;
            case 2: handle_member_menu(); break;
            case 3: handle_borrowing_menu(); break;
            case 0: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// Book management menu and operations
void handle_book_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Book Management ===\n");
        printf("1. Add New Book\n");
        printf("2. Update Book\n");
        printf("3. Delete Book\n");
        printf("4. View Book\n");
        printf("5. List All Books\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        switch (choice) {
            case 1: {
                // Add new book
                Book book;
                printf("Enter book details:\n");
                printf("ID: ");
                scanf("%d", &book.book_id);
                getchar();  // Clear input buffer
                get_input(book.title, "Title: ");
                get_input(book.author, "Author: ");
                get_input(book.isbn, "ISBN: ");
                printf("Copies Available: ");
                scanf("%d", &book.copies_available);
                getchar();  // Clear input buffer
                
                if (add_book(book)) {
                    printf("Book added successfully!\n");
                } else {
                    printf("Failed to add book.\n");
                }
                break;
            }
            case 2: {
                // Update book
                int book_id;
                printf("Enter book ID to update: ");
                scanf("%d", &book_id);
                getchar();  // Clear input buffer
                
                Book* book = get_book(book_id);
                if (book) {
                    printf("Enter new details (press Enter to keep current value):\n");
                    char input[MAX_STRING_LENGTH];
                    get_input(input, "Title: ");
                    if (strlen(input) > 0) strcpy(book->title, input);
                    get_input(input, "Author: ");
                    if (strlen(input) > 0) strcpy(book->author, input);
                    get_input(input, "ISBN: ");
                    if (strlen(input) > 0) strcpy(book->isbn, input);
                    printf("Copies Available: ");
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) book->copies_available = atoi(input);
                    if (update_book(*book)) {
                        printf("Book updated successfully!\n");
                    } else {
                        printf("Failed to update book.\n");
                    }
                    free(book);
                } else {
                    printf("Book not found!\n");
                }
                break;
            }
            case 3: {
                // Delete book
                int book_id;
                printf("Enter book ID to delete: ");
                scanf("%d", &book_id);
                getchar();  // Clear input buffer
                
                if (delete_book(book_id)) {
                    printf("Book deleted successfully!\n");
                } else {
                    printf("Failed to delete book.\n");
                }
                break;
            }
            case 4: {
                // View book
                int book_id;
                printf("Enter book ID to view: ");
                scanf("%d", &book_id);
                getchar();  // Clear input buffer
                Book* book = get_book(book_id);
                if (book) {
                    printf("\nBook Details:\n");
                    printf("ID: %d\n", book->book_id);
                    printf("Title: %s\n", book->title);
                    printf("Author: %s\n", book->author);
                    printf("ISBN: %s\n", book->isbn);
                    printf("Copies Available: %d\n", book->copies_available);
                    free(book);
                } else {
                    printf("Book not found!\n");
                }
                break;
            }
            case 5: {
                // List all books
                int count;
                Book* books = get_all_books(&count);
                if (books) {
                    printf("\nAll Books:\n");
                    for (int i = 0; i < count; i++) {
                        printf("\nBook %d:\n", i + 1);
                        printf("ID: %d\n", books[i].book_id);
                        printf("Title: %s\n", books[i].title);
                        printf("Author: %s\n", books[i].author);
                        printf("ISBN: %s\n", books[i].isbn);
                        printf("Copies Available: %d\n", books[i].copies_available);
                    }
                    free(books);
                } else {
                    printf("No books found!\n");
                }
                break;
            }
            case 0: return;
            default: printf("Invalid choice!\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// Author management menu
void handle_author_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Author Management ===\n");
        printf("1. Add New Author\n");
        printf("2. Update Author\n");
        printf("3. Delete Author\n");
        printf("4. View Author\n");
        printf("5. List All Authors\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        // Similar implementation as handle_book_menu
        // but for authors
        printf("\nAuthor management functionality coming soon!\n");
        printf("\nPress Enter to continue...");
        getchar();
        if (choice == 0) return;
    }
}

// Publisher management menu
void handle_publisher_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Publisher Management ===\n");
        printf("1. Add New Publisher\n");
        printf("2. Update Publisher\n");
        printf("3. Delete Publisher\n");
        printf("4. View Publisher\n");
        printf("5. List All Publishers\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        // Similar implementation as handle_book_menu
        // but for publishers
        printf("\nPublisher management functionality coming soon!\n");
        printf("\nPress Enter to continue...");
        getchar();
        if (choice == 0) return;
    }
}

// Member management menu
void handle_member_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Member Management ===\n");
        printf("1. Add New Member\n");
        printf("2. Update Member\n");
        printf("3. Delete Member\n");
        printf("4. View Member\n");
        printf("5. List All Members\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        switch (choice) {
            case 1: {
                // Add new member
                Member member;
                printf("Enter member details:\n");
                printf("ID: ");
                scanf("%d", &member.member_id);
                getchar();  // Clear input buffer
                get_input(member.name, "Name: ");
                get_input(member.phone, "Phone: ");
                get_input(member.email, "Email: ");
                
                if (add_member(member)) {
                    printf("Member added successfully!\n");
                } else {
                    printf("Failed to add member.\n");
                }
                break;
            }
            case 2: {
                // Update member
                int member_id;
                printf("Enter member ID to update: ");
                scanf("%d", &member_id);
                getchar();  // Clear input buffer
                
                Member* member = get_member(member_id);
                if (member) {
                    printf("Enter new details (press Enter to keep current value):\n");
                    char input[MAX_STRING_LENGTH];
                    get_input(input, "Name: ");
                    if (strlen(input) > 0) strcpy(member->name, input);
                    get_input(input, "Phone: ");
                    if (strlen(input) > 0) strcpy(member->phone, input);
                    get_input(input, "Email: ");
                    if (strlen(input) > 0) strcpy(member->email, input);
                    if (update_member(*member)) {
                        printf("Member updated successfully!\n");
                    } else {
                        printf("Failed to update member.\n");
                    }
                    free(member);
                } else {
                    printf("Member not found!\n");
                }
                break;
            }
            case 3: {
                // Delete member
                int member_id;
                printf("Enter member ID to delete: ");
                scanf("%d", &member_id);
                getchar();  // Clear input buffer
                
                if (delete_member(member_id)) {
                    printf("Member deleted successfully!\n");
                } else {
                    printf("Failed to delete member.\n");
                }
                break;
            }
            case 4: {
                // View member
                int member_id;
                printf("Enter member ID to view: ");
                scanf("%d", &member_id);
                getchar();  // Clear input buffer
                
                Member* member = get_member(member_id);
                if (member) {
                    printf("\nMember Details:\n");
                    printf("ID: %d\n", member->member_id);
                    printf("Name: %s\n", member->name);
                    printf("Phone: %s\n", member->phone);
                    printf("Email: %s\n", member->email);
                    free(member);
                } else {
                    printf("Member not found!\n");
                }
                break;
            }
            case 5: {
                // List all members
                int count;
                Member* members = get_all_members(&count);
                if (members) {
                    printf("\nAll Members:\n");
                    for (int i = 0; i < count; i++) {
                        printf("\nMember %d:\n", i + 1);
                        printf("ID: %d\n", members[i].member_id);
                        printf("Name: %s\n", members[i].name);
                        printf("Phone: %s\n", members[i].phone);
                        printf("Email: %s\n", members[i].email);
                    }
                    free(members);
                } else {
                    printf("No members found!\n");
                }
                break;
            }
            case 0: return;
            default: printf("Invalid choice!\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// Staff management menu
void handle_staff_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Staff Management ===\n");
        printf("1. Add New Staff\n");
        printf("2. Update Staff\n");
        printf("3. Delete Staff\n");
        printf("4. View Staff\n");
        printf("5. List All Staff\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        // Similar implementation as handle_book_menu
        // but for staff
        printf("\nStaff management functionality coming soon!\n");
        printf("\nPress Enter to continue...");
        getchar();
        if (choice == 0) return;
    }
}

// Borrowing management menu
void handle_borrowing_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Borrowing Management ===\n");
        printf("1. Add New Borrowing\n");
        printf("2. Update Borrowing\n");
        printf("3. Delete Borrowing\n");
        printf("4. View Borrowing\n");
        printf("5. List All Borrowings\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        switch (choice) {
            case 1: {
                // Add new borrowing
                Borrowing borrowing;
                printf("Enter borrowing details:\n");
                printf("ID: ");
                scanf("%d", &borrowing.borrowing_id);
                printf("Book ID: ");
                scanf("%d", &borrowing.book_id);
                printf("Member ID: ");
                scanf("%d", &borrowing.member_id);
                getchar();  // Clear input buffer
                get_input(borrowing.borrow_date, "Borrow Date (YYYY-MM-DD): ");
                get_input(borrowing.due_date, "Due Date (YYYY-MM-DD): ");
                get_input(borrowing.return_date, "Return Date (YYYY-MM-DD): ");
                
                if (add_borrowing(borrowing)) {
                    printf("Borrowing added successfully!\n");
                } else {
                    printf("Failed to add borrowing.\n");
                }
                break;
            }
            case 2: {
                // Update borrowing
                int borrowing_id;
                printf("Enter borrowing ID to update: ");
                scanf("%d", &borrowing_id);
                getchar();  // Clear input buffer
                
                Borrowing* borrowing = get_borrowing(borrowing_id);
                if (borrowing) {
                    printf("Enter new details (press Enter to keep current value):\n");
                    char input[MAX_STRING_LENGTH];
                    printf("Book ID: ");
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) borrowing->book_id = atoi(input);
                    printf("Member ID: ");
                    fgets(input, sizeof(input), stdin);
                    if (strlen(input) > 1) borrowing->member_id = atoi(input);
                    get_input(input, "Borrow Date (YYYY-MM-DD): ");
                    if (strlen(input) > 0) strcpy(borrowing->borrow_date, input);
                    get_input(input, "Due Date (YYYY-MM-DD): ");
                    if (strlen(input) > 0) strcpy(borrowing->due_date, input);
                    get_input(input, "Return Date (YYYY-MM-DD): ");
                    if (strlen(input) > 0) strcpy(borrowing->return_date, input);
                    if (update_borrowing(*borrowing)) {
                        printf("Borrowing updated successfully!\n");
                    } else {
                        printf("Failed to update borrowing.\n");
                    }
                    free(borrowing);
                } else {
                    printf("Borrowing not found!\n");
                }
                break;
            }
            case 3: {
                // Delete borrowing
                int borrowing_id;
                printf("Enter borrowing ID to delete: ");
                scanf("%d", &borrowing_id);
                getchar();  // Clear input buffer
                
                if (delete_borrowing(borrowing_id)) {
                    printf("Borrowing deleted successfully!\n");
                } else {
                    printf("Failed to delete borrowing.\n");
                }
                break;
            }
            case 4: {
                // View borrowing
                int borrowing_id;
                printf("Enter borrowing ID to view: ");
                scanf("%d", &borrowing_id);
                getchar();  // Clear input buffer
                
                Borrowing* borrowing = get_borrowing(borrowing_id);
                if (borrowing) {
                    printf("\nBorrowing Details:\n");
                    printf("ID: %d\n", borrowing->borrowing_id);
                    printf("Book ID: %d\n", borrowing->book_id);
                    printf("Member ID: %d\n", borrowing->member_id);
                    printf("Borrow Date: %s\n", borrowing->borrow_date);
                    printf("Due Date: %s\n", borrowing->due_date);
                    printf("Return Date: %s\n", borrowing->return_date);
                    free(borrowing);
                } else {
                    printf("Borrowing not found!\n");
                }
                break;
            }
            case 5: {
                // List all borrowings
                int count;
                Borrowing* borrowings = get_all_borrowings(&count);
                if (borrowings) {
                    printf("\nAll Borrowings:\n");
                    for (int i = 0; i < count; i++) {
                        printf("\nBorrowing %d:\n", i + 1);
                        printf("ID: %d\n", borrowings[i].borrowing_id);
                        printf("Book ID: %d\n", borrowings[i].book_id);
                        printf("Member ID: %d\n", borrowings[i].member_id);
                        printf("Borrow Date: %s\n", borrowings[i].borrow_date);
                        printf("Due Date: %s\n", borrowings[i].due_date);
                        printf("Return Date: %s\n", borrowings[i].return_date);
                    }
                    free(borrowings);
                } else {
                    printf("No borrowings found!\n");
                }
                break;
            }
            case 0: return;
            default: printf("Invalid choice!\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
}

// Fine management menu
void handle_fine_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Fine Management ===\n");
        printf("1. Add New Fine\n");
        printf("2. Update Fine\n");
        printf("3. Delete Fine\n");
        printf("4. View Fine\n");
        printf("5. List All Fines\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear input buffer
        
        // Similar implementation as handle_book_menu
        // but for fines
        printf("\nFine management functionality coming soon!\n");
        printf("\nPress Enter to continue...");
        getchar();
        if (choice == 0) return;
    }
} 