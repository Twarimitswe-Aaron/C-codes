#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../include/structures.h"
#include "../include/database.h"

// Color definitions
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_RESET   "\033[0m"

// Function declarations
void set_console_color(int color);
void display_menu();
void book_management_menu();
void handle_book_management();
void author_management_menu();
void handle_author_management();
void publisher_management_menu();
void handle_publisher_management();
void member_management_menu();
void handle_member_management();
void staff_management_menu();
void handle_staff_management();
void borrowing_management_menu();
void handle_borrowing_management();
void fine_management_menu();
void handle_fine_management();
void display_startup_menu();

void set_console_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void display_menu() {
    system("cls");  // Clear screen
    set_console_color(11);   // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |        LIBRARY MANAGEMENT MENU         |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Book Management                   |\n");
    printf("  |  [2] Author Management                 |\n");
    printf("  |  [3] Publisher Management              |\n");
    printf("  |  [4] Member Management                 |\n");
    printf("  |  [5] Staff Management                  |\n");
    printf("  |  [6] Borrowing Management              |\n");
    printf("  |  [7] Fine Management                   |\n");
    printf("  |  [0] Exit                              |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-7): ");
    set_console_color(7);   // White
}

void book_management_menu() {
    system("cls");  // Clear screen
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |            BOOK OPERATIONS             |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Add New Book                      |\n");
    printf("  |  [2] Update Book                       |\n");
    printf("  |  [3] Delete Book                       |\n");
    printf("  |  [4] List All Books                    |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_book_management() {
    int choice;
    Book book;
    int book_id;
    int count;
    Book* books;

    while (1) {
        book_management_menu();
        scanf("%d", &choice);
        getchar(); // Clear buffer

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         ADD NEW BOOK             |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter book title: ");
                set_console_color(7);
                fgets(book.title, sizeof(book.title), stdin);
                book.title[strcspn(book.title, "\n")] = 0;
                printf("  Enter author ID: ");
                scanf("%d", &book.author_id);
                printf("  Enter publisher ID: ");
                scanf("%d", &book.publisher_id);
                getchar();
                printf("  Enter ISBN: ");
                fgets(book.isbn, sizeof(book.isbn), stdin);
                book.isbn[strcspn(book.isbn, "\n")] = 0;
                printf("  Enter genre: ");
                fgets(book.genre, sizeof(book.genre), stdin);
                book.genre[strcspn(book.genre, "\n")] = 0;
                printf("  Enter year published: ");
                scanf("%d", &book.year_published);
                printf("  Enter copies available: ");
                scanf("%d", &book.copies_available);
                getchar();
                printf("  Enter shelf location: ");
                fgets(book.shelf_location, sizeof(book.shelf_location), stdin);
                book.shelf_location[strcspn(book.shelf_location, "\n")] = 0;
                if (add_book(book) == 0) {
                    set_console_color(10);  // Light Green
                    printf("\n  Book added successfully!\n");
                } else {
                    set_console_color(12);  // Light Red
                    printf("\n  Error adding book!\n");
                }
                set_console_color(7);   // White
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);  // Light Cyan
                printf("\n  +----------------------------------+\n");
                printf("  |         UPDATE BOOK              |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter book ID: ");
                set_console_color(7);   // White
                scanf("%d", &book_id);
                getchar();
                // Add update logic here
                break;

            case 3:
                set_console_color(11);  // Light Cyan
                printf("\n  +----------------------------------+\n");
                printf("  |         DELETE BOOK              |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter book ID: ");
                set_console_color(7);   // White
                scanf("%d", &book_id);
                if (delete_book(book_id) == 0) {
                    set_console_color(10);  // Light Green
                    printf("\n  Book deleted successfully!\n");
                } else {
                    set_console_color(12);  // Light Red
                    printf("\n  Error deleting book!\n");
                }
                set_console_color(7);   // White
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                books = get_all_books(&count);
                if (books != NULL) {
                    set_console_color(10);  // Light Green
                    printf("\n  +----------------------------------+\n");
                    printf("  |          BOOK LIST               |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);  // Light Cyan
                    for (int i = 0; i < count; i++) {
                        printf("  %d. %s (ISBN: %s, Genre: %s, Copies: %d)\n", 
                               books[i].book_id, 
                               books[i].title,
                               books[i].isbn,
                               books[i].genre,
                               books[i].copies_available);
                    }
                    free(books);
                } else {
                    set_console_color(12);  // Light Red
                    printf("\n  Error retrieving books!\n");
                }
                set_console_color(7);   // White
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);  // Light Red
                printf("\n  Invalid choice!\n");
                set_console_color(7);   // White
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

// Author Management Menu
void author_management_menu() {
    system("cls");
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |           AUTHOR OPERATIONS            |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Add New Author                    |\n");
    printf("  |  [2] Update Author                     |\n");
    printf("  |  [3] Delete Author                     |\n");
    printf("  |  [4] List All Authors                  |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_author_management() {
    int choice;
    Author author;
    int author_id;
    int count;
    Author* authors;

    while (1) {
        author_management_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         ADD NEW AUTHOR          |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter author name: ");
                set_console_color(7);
                fgets(author.name, sizeof(author.name), stdin);
                author.name[strcspn(author.name, "\n")] = 0;
                printf("  Enter author bio: ");
                fgets(author.bio, sizeof(author.bio), stdin);
                author.bio[strcspn(author.bio, "\n")] = 0;
                
                if (add_author(author) == 0) {
                    set_console_color(10);
                    printf("\n  Author added successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error adding author!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         UPDATE AUTHOR           |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter author ID: ");
                set_console_color(7);
                scanf("%d", &author_id);
                getchar();
                // Add update logic here
                break;

            case 3:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         DELETE AUTHOR           |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter author ID: ");
                set_console_color(7);
                scanf("%d", &author_id);
                if (delete_author(author_id) == 0) {
                    set_console_color(10);
                    printf("\n  Author deleted successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error deleting author!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                authors = get_all_authors(&count);
                if (authors != NULL) {
                    set_console_color(10);
                    printf("\n  +----------------------------------+\n");
                    printf("  |          AUTHOR LIST            |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);
                    for (int i = 0; i < count; i++) {
                        printf("  %d. %s\n", authors[i].author_id, authors[i].name);
                    }
                    free(authors);
                } else {
                    set_console_color(12);
                    printf("\n  Error retrieving authors!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

// Publisher Management Menu
void publisher_management_menu() {
    system("cls");
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |          PUBLISHER OPERATIONS          |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Add New Publisher                 |\n");
    printf("  |  [2] Update Publisher                  |\n");
    printf("  |  [3] Delete Publisher                  |\n");
    printf("  |  [4] List All Publishers               |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_publisher_management() {
    int choice;
    Publisher publisher;
    int publisher_id;
    int count;
    Publisher* publishers;

    while (1) {
        publisher_management_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         ADD NEW PUBLISHER        |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter publisher name: ");
                set_console_color(7);
                fgets(publisher.name, sizeof(publisher.name), stdin);
                publisher.name[strcspn(publisher.name, "\n")] = 0;
                printf("  Enter publisher address: ");
                fgets(publisher.address, sizeof(publisher.address), stdin);
                publisher.address[strcspn(publisher.address, "\n")] = 0;
                printf("  Enter contact info: ");
                fgets(publisher.contact_info, sizeof(publisher.contact_info), stdin);
                publisher.contact_info[strcspn(publisher.contact_info, "\n")] = 0;
                
                if (add_publisher(publisher) == 0) {
                    set_console_color(10);
                    printf("\n  Publisher added successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error adding publisher!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         UPDATE PUBLISHER        |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter publisher ID: ");
                set_console_color(7);
                scanf("%d", &publisher_id);
                getchar();
                // Add update logic here
                break;

            case 3:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         DELETE PUBLISHER        |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter publisher ID: ");
                set_console_color(7);
                scanf("%d", &publisher_id);
                if (delete_publisher(publisher_id) == 0) {
                    set_console_color(10);
                    printf("\n  Publisher deleted successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error deleting publisher!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                publishers = get_all_publishers(&count);
                if (publishers != NULL) {
                    set_console_color(10);
                    printf("\n  +----------------------------------+\n");
                    printf("  |          PUBLISHER LIST         |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);
                    for (int i = 0; i < count; i++) {
                        printf("  %d. %s (Contact: %s)\n", 
                               publishers[i].publisher_id, 
                               publishers[i].name,
                               publishers[i].contact_info);
                    }
                    free(publishers);
                } else {
                    set_console_color(12);
                    printf("\n  Error retrieving publishers!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

// Member Management Menu
void member_management_menu() {
    system("cls");
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |            MEMBER OPERATIONS           |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Add New Member                    |\n");
    printf("  |  [2] Update Member                     |\n");
    printf("  |  [3] Delete Member                     |\n");
    printf("  |  [4] List All Members                  |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_member_management() {
    int choice;
    Member member;
    int member_id;
    int count;
    Member* members;

    while (1) {
        member_management_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |        ADD NEW MEMBER           |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter member name: ");
                set_console_color(7);
                fgets(member.name, sizeof(member.name), stdin);
                member.name[strcspn(member.name, "\n")] = 0;
                printf("  Enter member address: ");
                fgets(member.address, sizeof(member.address), stdin);
                member.address[strcspn(member.address, "\n")] = 0;
                printf("  Enter member phone: ");
                fgets(member.phone, sizeof(member.phone), stdin);
                member.phone[strcspn(member.phone, "\n")] = 0;
                printf("  Enter member email: ");
                fgets(member.email, sizeof(member.email), stdin);
                member.email[strcspn(member.email, "\n")] = 0;
                member.date_joined = time(NULL);
                strcpy(member.membership_status, "ACTIVE");
                
                if (add_member(member) == 0) {
                    set_console_color(10);
                    printf("\n  Member added successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error adding member!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |        UPDATE MEMBER            |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter member ID: ");
                set_console_color(7);
                scanf("%d", &member_id);
                getchar();
                // Add update logic here
                break;

            case 3:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |        DELETE MEMBER            |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter member ID: ");
                set_console_color(7);
                scanf("%d", &member_id);
                if (delete_member(member_id) == 0) {
                    set_console_color(10);
                    printf("\n  Member deleted successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error deleting member!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                members = get_all_members(&count);
                if (members != NULL) {
                    set_console_color(10);
                    printf("\n  +----------------------------------+\n");
                    printf("  |         MEMBER LIST             |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);
                    for (int i = 0; i < count; i++) {
                        printf("  %d. %s (Email: %s, Status: %s)\n", 
                               members[i].member_id, 
                               members[i].name,
                               members[i].email,
                               members[i].membership_status);
                    }
                    free(members);
                } else {
                    set_console_color(12);
                    printf("\n  Error retrieving members!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

// Staff Management Menu
void staff_management_menu() {
    system("cls");
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |             STAFF OPERATIONS           |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Add New Staff                     |\n");
    printf("  |  [2] Update Staff                      |\n");
    printf("  |  [3] Delete Staff                      |\n");
    printf("  |  [4] List All Staff                    |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_staff_management() {
    int choice;
    Staff staff;
    int staff_id;
    int count;
    Staff* staff_list;

    while (1) {
        staff_management_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         ADD NEW STAFF           |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter staff name: ");
                set_console_color(7);
                fgets(staff.name, sizeof(staff.name), stdin);
                staff.name[strcspn(staff.name, "\n")] = 0;
                printf("  Enter staff role: ");
                fgets(staff.role, sizeof(staff.role), stdin);
                staff.role[strcspn(staff.role, "\n")] = 0;
                printf("  Enter staff email: ");
                fgets(staff.email, sizeof(staff.email), stdin);
                staff.email[strcspn(staff.email, "\n")] = 0;
                printf("  Enter staff phone: ");
                fgets(staff.phone, sizeof(staff.phone), stdin);
                staff.phone[strcspn(staff.phone, "\n")] = 0;
                
                if (add_staff(staff) == 0) {
                    set_console_color(10);
                    printf("\n  Staff added successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error adding staff!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         UPDATE STAFF            |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter staff ID: ");
                set_console_color(7);
                scanf("%d", &staff_id);
                getchar();
                // Add update logic here
                break;

            case 3:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         DELETE STAFF            |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter staff ID: ");
                set_console_color(7);
                scanf("%d", &staff_id);
                if (delete_staff(staff_id) == 0) {
                    set_console_color(10);
                    printf("\n  Staff deleted successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error deleting staff!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                staff_list = get_all_staff(&count);
                if (staff_list != NULL) {
                    set_console_color(10);
                    printf("\n  +----------------------------------+\n");
                    printf("  |          STAFF LIST             |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);
                    for (int i = 0; i < count; i++) {
                        printf("  %d. %s (Role: %s, Email: %s)\n", 
                               staff_list[i].staff_id, 
                               staff_list[i].name,
                               staff_list[i].role,
                               staff_list[i].email);
                    }
                    free(staff_list);
                } else {
                    set_console_color(12);
                    printf("\n  Error retrieving staff list!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

// Borrowing Management Menu
void borrowing_management_menu() {
    system("cls");
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |          BORROWING OPERATIONS          |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] New Borrowing                     |\n");
    printf("  |  [2] Return Book                       |\n");
    printf("  |  [3] List Active Borrowings           |\n");
    printf("  |  [4] List All Borrowings              |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_borrowing_management() {
    int choice;
    Borrowing borrowing;
    int borrowing_id;
    int count;
    Borrowing* borrowings;

    while (1) {
        borrowing_management_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         NEW BORROWING           |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter book ID: ");
                set_console_color(7);
                scanf("%d", &borrowing.book_id);
                printf("  Enter member ID: ");
                scanf("%d", &borrowing.member_id);
                printf("  Enter staff ID: ");
                scanf("%d", &borrowing.staff_id);
                getchar();
                borrowing.borrow_date = time(NULL);
                borrowing.due_date = borrowing.borrow_date + (14 * 24 * 60 * 60); // 14 days from now
                borrowing.return_date = 0; // Not returned yet
                
                if (add_borrowing(borrowing) == 0) {
                    set_console_color(10);
                    printf("\n  Book borrowed successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error borrowing book!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |         RETURN BOOK             |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter borrowing ID: ");
                set_console_color(7);
                scanf("%d", &borrowing_id);
                // Add return logic here
                break;

            case 3:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |        VIEW BORROWING           |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter borrowing ID: ");
                set_console_color(7);
                scanf("%d", &borrowing_id);
                Borrowing* found_borrowing = get_borrowing(borrowing_id);
                if (found_borrowing != NULL) {
                    set_console_color(10);
                    printf("\n  Borrowing Details:\n");
                    set_console_color(11);
                    printf("  ID: %d\n", found_borrowing->borrowing_id);
                    printf("  Book ID: %d\n", found_borrowing->book_id);
                    printf("  Member ID: %d\n", found_borrowing->member_id);
                    printf("  Staff ID: %d\n", found_borrowing->staff_id);
                    printf("  Borrow Date: %s", ctime(&found_borrowing->borrow_date));
                    printf("  Due Date: %s", ctime(&found_borrowing->due_date));
                    if (found_borrowing->return_date != 0) {
                        printf("  Return Date: %s", ctime(&found_borrowing->return_date));
                    } else {
                        printf("  Return Date: Not returned yet\n");
                    }
                    free(found_borrowing);
                } else {
                    set_console_color(12);
                    printf("\n  Borrowing not found!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                borrowings = get_all_borrowings(&count);
                if (borrowings != NULL) {
                    set_console_color(10);
                    printf("\n  +----------------------------------+\n");
                    printf("  |        BORROWING LIST           |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);
                    for (int i = 0; i < count; i++) {
                        if (borrowings[i].return_date != 0) continue;
                        printf("  %d. Book ID: %d, Member ID: %d (Borrowed: %s)\n", 
                               borrowings[i].borrowing_id, 
                               borrowings[i].book_id, 
                               borrowings[i].member_id,
                               ctime(&borrowings[i].borrow_date));
                    }
                    free(borrowings);
                } else {
                    set_console_color(12);
                    printf("\n  Error retrieving borrowings!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

// Fine Management Menu
void fine_management_menu() {
    system("cls");
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |             FINE OPERATIONS            |\n");
    printf("  |                                        |\n");
    printf("  |  Available Operations:                 |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Add Fine                          |\n");
    printf("  |  [2] Update Fine                       |\n");
    printf("  |  [3] List Unpaid Fines                 |\n");
    printf("  |  [4] List All Fines                    |\n");
    printf("  |  [0] Back to Main Menu                 |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-4): ");
    set_console_color(7);   // White
}

void handle_fine_management() {
    int choice;
    Fine fine;
    int fine_id;
    int count;
    Fine* fines;

    while (1) {
        fine_management_menu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |           ADD FINE              |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter borrowing ID: ");
                set_console_color(7);
                scanf("%d", &fine.borrowing_id);
                printf("  Enter fine amount: ");
                scanf("%f", &fine.amount);
                getchar();
                fine.paid = 0; // Not paid yet
                fine.date_paid = 0; // Not paid yet
                
                if (add_fine(fine) == 0) {
                    set_console_color(10);
                    printf("\n  Fine added successfully!\n");
                } else {
                    set_console_color(12);
                    printf("\n  Error adding fine!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 2:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |          UPDATE FINE            |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter fine ID: ");
                set_console_color(7);
                scanf("%d", &fine_id);
                // Add update logic here
                break;

            case 3:
                set_console_color(11);
                printf("\n  +----------------------------------+\n");
                printf("  |           VIEW FINE             |\n");
                printf("  +----------------------------------+\n\n");
                printf("  Enter fine ID: ");
                set_console_color(7);
                scanf("%d", &fine_id);
                Fine* found_fine = get_fine(fine_id);
                if (found_fine != NULL) {
                    set_console_color(10);
                    printf("\n  Fine Details:\n");
                    set_console_color(11);
                    printf("  ID: %d\n", found_fine->fine_id);
                    printf("  Borrowing ID: %d\n", found_fine->borrowing_id);
                    printf("  Amount: $%.2f\n", found_fine->amount);
                    printf("  Status: %s\n", found_fine->paid ? "PAID" : "UNPAID");
                    if (found_fine->date_paid != 0) {
                        printf("  Date Paid: %s", ctime(&found_fine->date_paid));
                    } else {
                        printf("  Date Paid: Not paid yet\n");
                    }
                    free(found_fine);
                } else {
                    set_console_color(12);
                    printf("\n  Fine not found!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 4:
                fines = get_all_fines(&count);
                if (fines != NULL) {
                    set_console_color(10);
                    printf("\n  +----------------------------------+\n");
                    printf("  |           FINE LIST             |\n");
                    printf("  +----------------------------------+\n\n");
                    set_console_color(11);
                    for (int i = 0; i < count; i++) {
                        if (fines[i].paid) continue;
                        printf("  %d. Borrowing ID: %d, Amount: $%.2f, Status: %s\n", 
                               fines[i].fine_id, 
                               fines[i].borrowing_id, 
                               fines[i].amount,
                               fines[i].paid ? "PAID" : "UNPAID");
                    }
                    free(fines);
                } else {
                    set_console_color(12);
                    printf("\n  Error retrieving fines!\n");
                }
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:
                return;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }
}

void display_startup_menu() {
    system("cls");  // Clear screen
    set_console_color(11);  // Light Cyan
    printf("\n");
    printf("  ==========================================\n");
    printf("  |                                        |\n");
    printf("  |        LIBRARY MANAGEMENT SYSTEM       |\n");
    printf("  |                                        |\n");
    printf("  |  Select Interface Mode:                |\n");
    printf("  |                                        |\n");
    set_console_color(14);  // Yellow
    printf("  |  [1] Console Mode                      |\n");
    printf("  |  [2] GUI Mode                          |\n");
    printf("  |  [0] Exit                              |\n");
    set_console_color(11);  // Light Cyan
    printf("  |                                        |\n");
    printf("  ==========================================\n\n");
    set_console_color(14);  // Yellow
    printf("  Enter choice (0-2): ");
    set_console_color(7);   // White
}

int main() {
    int choice;
    int interface_choice;

    // Initialize database
    init_database();

    while (1) {
        display_startup_menu();
        scanf("%d", &interface_choice);
        getchar();

        switch (interface_choice) {
            case 1:  // Console Mode
                while (1) {
                    display_menu();
                    scanf("%d", &choice);
                    getchar();

                    switch (choice) {
                        case 1:
                            handle_book_management();
                            break;

                        case 2:
                            handle_author_management();
                            break;

                        case 3:
                            handle_publisher_management();
                            break;

                        case 4:
                            handle_member_management();
                            break;

                        case 5:
                            handle_staff_management();
                            break;

                        case 6:
                            handle_borrowing_management();
                            break;

                        case 7:
                            handle_fine_management();
                            break;

                        case 0:
                            close_database();
                            set_console_color(10);
                            printf("\n  Thank you for using the Library Management System!\n");
                            set_console_color(7);
                            return 0;

                        default:
                            set_console_color(12);
                            printf("\n  Invalid choice!\n");
                            set_console_color(7);
                            printf("\n  Press Enter to continue...");
                            getchar();
                    }
                }
                break;

            case 2:  // GUI Mode
                set_console_color(12);
                printf("\n  GUI Mode is not implemented yet.\n");
                printf("  Please use Console Mode for now.\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
                break;

            case 0:  // Exit
                close_database();
                set_console_color(10);
                printf("\n  Thank you for using the Library Management System!\n");
                set_console_color(7);
                return 0;

            default:
                set_console_color(12);
                printf("\n  Invalid choice!\n");
                set_console_color(7);
                printf("\n  Press Enter to continue...");
                getchar();
        }
    }

    return 0;
} 