#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "console.h"
#include "database.h"
#include "structures.h"
#include "date_utils.h"

// Helper functions
void clearScreen() {
    system("cls");
}

void waitForKey() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void printHeader(const char* title) {
    clearScreen();
    printf("\n=== %s ===\n\n", title);
}

void printError(const char* message) {
    printf("\nError: %s\n", message);
}

void printSuccess(const char* message) {
    printf("\nSuccess: %s\n", message);
}

// Main console functions
void startConsole() {
    showConsoleMainMenu();
}

void showConsoleMainMenu() {
    int choice;
    do {
        printHeader("Library Management System");
        printf("1. Book Management\n");
        printf("2. Author Management\n");
        printf("3. Publisher Management\n");
        printf("4. Member Management\n");
        printf("5. Staff Management\n");
        printf("6. Borrowing Management\n");
        printf("7. Fine Management\n");
        printf("8. Exit\n");
        printf("\nEnter your choice (1-8): ");
        
        scanf("%d", &choice);
        getchar(); // Clear input buffer
        
        switch(choice) {
            case 1: showBookMenu(); break;
            case 2: showAuthorMenu(); break;
            case 3: showPublisherMenu(); break;
            case 4: showMemberMenu(); break;
            case 5: showStaffMenu(); break;
            case 6: showBorrowingMenu(); break;
            case 7: showFineMenu(); break;
            case 8: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

// Book menu and operations
void showBookMenu() {
    int choice;
    do {
        printHeader("Book Management");
        printf("1. Add Book\n");
        printf("2. Update Book\n");
        printf("3. Delete Book\n");
        printf("4. View Book\n");
        printf("5. List All Books\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar(); // Clear input buffer
        
        switch(choice) {
            case 1: addBookConsole(); break;
            case 2: updateBookConsole(); break;
            case 3: deleteBookConsole(); break;
            case 4: viewBookConsole(); break;
            case 5: listAllBooksConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addBookConsole() {
    printHeader("Add New Book");
    
    Book book;
    printf("Enter book details:\n");
    
    printf("Title: ");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = 0;
    
    printf("Author ID: ");
    scanf("%d", &book.author_id);
    getchar();
    
    printf("Publisher ID: ");
    scanf("%d", &book.publisher_id);
    getchar();
    
    printf("ISBN: ");
    fgets(book.isbn, sizeof(book.isbn), stdin);
    book.isbn[strcspn(book.isbn, "\n")] = 0;
    
    printf("Genre: ");
    fgets(book.genre, sizeof(book.genre), stdin);
    book.genre[strcspn(book.genre, "\n")] = 0;
    
    printf("Year Published: ");
    scanf("%d", &book.year_published);
    getchar();
    
    printf("Copies Available: ");
    scanf("%d", &book.copies_available);
    getchar();
    
    printf("Shelf Location: ");
    fgets(book.shelf_location, sizeof(book.shelf_location), stdin);
    book.shelf_location[strcspn(book.shelf_location, "\n")] = 0;
    
    if (addBook(book)) {
        printSuccess("Book added successfully!");
    } else {
        printError("Failed to add book!");
    }
    
    waitForKey();
}

void updateBookConsole() {
    printHeader("Update Book");
    
    int book_id;
    printf("Enter Book ID to update: ");
    scanf("%d", &book_id);
    getchar();
    
    Book* book = getBook(book_id);
    if (!book) {
        printError("Book not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Title [%s]: ", book->title);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(book->title, input);
    
    printf("Author ID [%d]: ", book->author_id);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) book->author_id = atoi(input);
    
    printf("Publisher ID [%d]: ", book->publisher_id);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) book->publisher_id = atoi(input);
    
    printf("ISBN [%s]: ", book->isbn);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(book->isbn, input);
    
    printf("Genre [%s]: ", book->genre);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(book->genre, input);
    
    printf("Year Published [%d]: ", book->year_published);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) book->year_published = atoi(input);
    
    printf("Copies Available [%d]: ", book->copies_available);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) book->copies_available = atoi(input);
    
    printf("Shelf Location [%s]: ", book->shelf_location);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(book->shelf_location, input);
    
    if (updateBook(*book)) {
        printSuccess("Book updated successfully!");
    } else {
        printError("Failed to update book!");
    }
    
    free(book);
    waitForKey();
}

void deleteBookConsole() {
    printHeader("Delete Book");
    
    int book_id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &book_id);
    getchar();
    
    if (deleteBook(book_id)) {
        printSuccess("Book deleted successfully!");
    } else {
        printError("Failed to delete book!");
    }
    
    waitForKey();
}

void viewBookConsole() {
    printHeader("View Book");
    
    int book_id;
    printf("Enter Book ID to view: ");
    scanf("%d", &book_id);
    getchar();
    
    Book* book = getBook(book_id);
    if (!book) {
        printError("Book not found!");
        waitForKey();
        return;
    }
    
    printf("\nBook Details:\n");
    printf("ID: %d\n", book->book_id);
    printf("Title: %s\n", book->title);
    printf("Author ID: %d\n", book->author_id);
    printf("Publisher ID: %d\n", book->publisher_id);
    printf("ISBN: %s\n", book->isbn);
    printf("Genre: %s\n", book->genre);
    printf("Year Published: %d\n", book->year_published);
    printf("Copies Available: %d\n", book->copies_available);
    printf("Shelf Location: %s\n", book->shelf_location);
    
    free(book);
    waitForKey();
}

void listAllBooksConsole() {
    printHeader("All Books");
    
    int count;
    Book* books = getAllBooks(&count);
    
    if (!books) {
        printError("Failed to retrieve books!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No books found.\n");
    } else {
        printf("%-5s %-30s %-8s %-8s %-15s %-15s %-6s %-8s %-15s\n", 
               "ID", "Title", "Author ID", "Pub ID", "ISBN", "Genre", "Year", "Copies", "Shelf Location");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            printf("%-5d %-30s %-8d %-8d %-15s %-15s %-6d %-8d %-15s\n",
                   books[i].book_id,
                   books[i].title,
                   books[i].author_id,
                   books[i].publisher_id,
                   books[i].isbn,
                   books[i].genre,
                   books[i].year_published,
                   books[i].copies_available,
                   books[i].shelf_location);
        }
    }
    
    free(books);
    waitForKey();
}

// Author menu and operations
void showAuthorMenu() {
    int choice;
    do {
        printHeader("Author Management");
        printf("1. Add Author\n");
        printf("2. Update Author\n");
        printf("3. Delete Author\n");
        printf("4. View Author\n");
        printf("5. List All Authors\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: addAuthorConsole(); break;
            case 2: updateAuthorConsole(); break;
            case 3: deleteAuthorConsole(); break;
            case 4: viewAuthorConsole(); break;
            case 5: listAllAuthorsConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addAuthorConsole() {
    printHeader("Add New Author");
    
    Author author;
    printf("Enter author details:\n");
    
    printf("Name: ");
    fgets(author.name, sizeof(author.name), stdin);
    author.name[strcspn(author.name, "\n")] = 0;
    
    printf("Biography: ");
    fgets(author.bio, sizeof(author.bio), stdin);
    author.bio[strcspn(author.bio, "\n")] = 0;
    
    if (addAuthor(author)) {
        printSuccess("Author added successfully!");
    } else {
        printError("Failed to add author!");
    }
    
    waitForKey();
}

void updateAuthorConsole() {
    printHeader("Update Author");
    
    int author_id;
    printf("Enter Author ID to update: ");
    scanf("%d", &author_id);
    getchar();
    
    Author* author = getAuthor(author_id);
    if (!author) {
        printError("Author not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Name [%s]: ", author->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(author->name, input);
    
    printf("Biography [%s]: ", author->bio);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(author->bio, input);
    
    if (updateAuthor(*author)) {
        printSuccess("Author updated successfully!");
    } else {
        printError("Failed to update author!");
    }
    
    free(author);
    waitForKey();
}

void deleteAuthorConsole() {
    printHeader("Delete Author");
    
    int author_id;
    printf("Enter Author ID to delete: ");
    scanf("%d", &author_id);
    getchar();
    
    if (deleteAuthor(author_id)) {
        printSuccess("Author deleted successfully!");
    } else {
        printError("Failed to delete author!");
    }
    
    waitForKey();
}

void viewAuthorConsole() {
    printHeader("View Author");
    
    int author_id;
    printf("Enter Author ID to view: ");
    scanf("%d", &author_id);
    getchar();
    
    Author* author = getAuthor(author_id);
    if (!author) {
        printError("Author not found!");
        waitForKey();
        return;
    }
    
    printf("\nAuthor Details:\n");
    printf("ID: %d\n", author->author_id);
    printf("Name: %s\n", author->name);
    printf("Biography: %s\n", author->bio);
    
    free(author);
    waitForKey();
}

void listAllAuthorsConsole() {
    printHeader("All Authors");
    
    int count;
    Author* authors = getAllAuthors(&count);
    
    if (!authors) {
        printError("Failed to retrieve authors!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No authors found.\n");
    } else {
        printf("%-5s %-30s %-50s\n", "ID", "Name", "Biography");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            printf("%-5d %-30s %-50s\n",
                   authors[i].author_id,
                   authors[i].name,
                   authors[i].bio);
        }
    }
    
    free(authors);
    waitForKey();
}

// Publisher menu and operations
void showPublisherMenu() {
    int choice;
    do {
        printHeader("Publisher Management");
        printf("1. Add Publisher\n");
        printf("2. Update Publisher\n");
        printf("3. Delete Publisher\n");
        printf("4. View Publisher\n");
        printf("5. List All Publishers\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: addPublisherConsole(); break;
            case 2: updatePublisherConsole(); break;
            case 3: deletePublisherConsole(); break;
            case 4: viewPublisherConsole(); break;
            case 5: listAllPublishersConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addPublisherConsole() {
    printHeader("Add New Publisher");
    
    Publisher publisher;
    printf("Enter publisher details:\n");
    
    printf("Name: ");
    fgets(publisher.name, sizeof(publisher.name), stdin);
    publisher.name[strcspn(publisher.name, "\n")] = 0;
    
    printf("Address: ");
    fgets(publisher.address, sizeof(publisher.address), stdin);
    publisher.address[strcspn(publisher.address, "\n")] = 0;
    
    printf("Contact Info: ");
    fgets(publisher.contact_info, sizeof(publisher.contact_info), stdin);
    publisher.contact_info[strcspn(publisher.contact_info, "\n")] = 0;
    
    if (addPublisher(publisher)) {
        printSuccess("Publisher added successfully!");
    } else {
        printError("Failed to add publisher!");
    }
    
    waitForKey();
}

void updatePublisherConsole() {
    printHeader("Update Publisher");
    
    int publisher_id;
    printf("Enter Publisher ID to update: ");
    scanf("%d", &publisher_id);
    getchar();
    
    Publisher* publisher = getPublisher(publisher_id);
    if (!publisher) {
        printError("Publisher not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Name [%s]: ", publisher->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(publisher->name, input);
    
    printf("Address [%s]: ", publisher->address);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(publisher->address, input);
    
    printf("Contact Info [%s]: ", publisher->contact_info);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(publisher->contact_info, input);
    
    if (updatePublisher(*publisher)) {
        printSuccess("Publisher updated successfully!");
    } else {
        printError("Failed to update publisher!");
    }
    
    free(publisher);
    waitForKey();
}

void deletePublisherConsole() {
    printHeader("Delete Publisher");
    
    int publisher_id;
    printf("Enter Publisher ID to delete: ");
    scanf("%d", &publisher_id);
    getchar();
    
    if (deletePublisher(publisher_id)) {
        printSuccess("Publisher deleted successfully!");
    } else {
        printError("Failed to delete publisher!");
    }
    
    waitForKey();
}

void viewPublisherConsole() {
    printHeader("View Publisher");
    
    int publisher_id;
    printf("Enter Publisher ID to view: ");
    scanf("%d", &publisher_id);
    getchar();
    
    Publisher* publisher = getPublisher(publisher_id);
    if (!publisher) {
        printError("Publisher not found!");
        waitForKey();
        return;
    }
    
    printf("\nPublisher Details:\n");
    printf("ID: %d\n", publisher->publisher_id);
    printf("Name: %s\n", publisher->name);
    printf("Address: %s\n", publisher->address);
    printf("Contact Info: %s\n", publisher->contact_info);
    
    free(publisher);
    waitForKey();
}

void listAllPublishersConsole() {
    printHeader("All Publishers");
    
    int count;
    Publisher* publishers = getAllPublishers(&count);
    
    if (!publishers) {
        printError("Failed to retrieve publishers!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No publishers found.\n");
    } else {
        printf("%-5s %-30s %-40s %-30s\n", "ID", "Name", "Address", "Contact Info");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            printf("%-5d %-30s %-40s %-30s\n",
                   publishers[i].publisher_id,
                   publishers[i].name,
                   publishers[i].address,
                   publishers[i].contact_info);
        }
    }
    
    free(publishers);
    waitForKey();
}

// Member menu and operations
void showMemberMenu() {
    int choice;
    do {
        printHeader("Member Management");
        printf("1. Add Member\n");
        printf("2. Update Member\n");
        printf("3. Delete Member\n");
        printf("4. View Member\n");
        printf("5. List All Members\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: addMemberConsole(); break;
            case 2: updateMemberConsole(); break;
            case 3: deleteMemberConsole(); break;
            case 4: viewMemberConsole(); break;
            case 5: listAllMembersConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addMemberConsole() {
    printHeader("Add New Member");
    
    Member member;
    printf("Enter member details:\n");
    
    printf("Name: ");
    fgets(member.name, sizeof(member.name), stdin);
    member.name[strcspn(member.name, "\n")] = 0;
    
    printf("Email: ");
    fgets(member.email, sizeof(member.email), stdin);
    member.email[strcspn(member.email, "\n")] = 0;
    
    printf("Phone: ");
    fgets(member.phone, sizeof(member.phone), stdin);
    member.phone[strcspn(member.phone, "\n")] = 0;
    
    printf("Address: ");
    fgets(member.address, sizeof(member.address), stdin);
    member.address[strcspn(member.address, "\n")] = 0;
    
    if (addMember(member)) {
        printSuccess("Member added successfully!");
    } else {
        printError("Failed to add member!");
    }
    
    waitForKey();
}

void updateMemberConsole() {
    printHeader("Update Member");
    
    int member_id;
    printf("Enter Member ID to update: ");
    scanf("%d", &member_id);
    getchar();
    
    Member* member = getMember(member_id);
    if (!member) {
        printError("Member not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Name [%s]: ", member->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(member->name, input);
    
    printf("Email [%s]: ", member->email);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(member->email, input);
    
    printf("Phone [%s]: ", member->phone);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(member->phone, input);
    
    printf("Address [%s]: ", member->address);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(member->address, input);
    
    if (updateMember(*member)) {
        printSuccess("Member updated successfully!");
    } else {
        printError("Failed to update member!");
    }
    
    free(member);
    waitForKey();
}

void deleteMemberConsole() {
    printHeader("Delete Member");
    
    int member_id;
    printf("Enter Member ID to delete: ");
    scanf("%d", &member_id);
    getchar();
    
    if (deleteMember(member_id)) {
        printSuccess("Member deleted successfully!");
    } else {
        printError("Failed to delete member!");
    }
    
    waitForKey();
}

void viewMemberConsole() {
    printHeader("View Member");
    
    int member_id;
    printf("Enter Member ID to view: ");
    scanf("%d", &member_id);
    getchar();
    
    Member* member = getMember(member_id);
    if (!member) {
        printError("Member not found!");
        waitForKey();
        return;
    }
    
    printf("\nMember Details:\n");
    printf("ID: %d\n", member->member_id);
    printf("Name: %s\n", member->name);
    printf("Email: %s\n", member->email);
    printf("Phone: %s\n", member->phone);
    printf("Address: %s\n", member->address);
    
    free(member);
    waitForKey();
}

void listAllMembersConsole() {
    printHeader("All Members");
    
    int count;
    Member* members = getAllMembers(&count);
    
    if (!members) {
        printError("Failed to retrieve members!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No members found.\n");
    } else {
        printf("%-5s %-30s %-30s %-15s %-40s\n", "ID", "Name", "Email", "Phone", "Address");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            printf("%-5d %-30s %-30s %-15s %-40s\n",
                   members[i].member_id,
                   members[i].name,
                   members[i].email,
                   members[i].phone,
                   members[i].address);
        }
    }
    
    free(members);
    waitForKey();
}

// Staff menu and operations
void showStaffMenu() {
    int choice;
    do {
        printHeader("Staff Management");
        printf("1. Add Staff\n");
        printf("2. Update Staff\n");
        printf("3. Delete Staff\n");
        printf("4. View Staff\n");
        printf("5. List All Staff\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: addStaffConsole(); break;
            case 2: updateStaffConsole(); break;
            case 3: deleteStaffConsole(); break;
            case 4: viewStaffConsole(); break;
            case 5: listAllStaffConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addStaffConsole() {
    printHeader("Add New Staff");
    
    Staff staff;
    printf("Enter staff details:\n");
    
    printf("Name: ");
    fgets(staff.name, sizeof(staff.name), stdin);
    staff.name[strcspn(staff.name, "\n")] = 0;
    
    printf("Email: ");
    fgets(staff.email, sizeof(staff.email), stdin);
    staff.email[strcspn(staff.email, "\n")] = 0;
    
    printf("Phone: ");
    fgets(staff.phone, sizeof(staff.phone), stdin);
    staff.phone[strcspn(staff.phone, "\n")] = 0;
    
    if (addStaff(staff)) {
        printSuccess("Staff added successfully!");
    } else {
        printError("Failed to add staff!");
    }
    
    waitForKey();
}

void updateStaffConsole() {
    printHeader("Update Staff");
    
    int staff_id;
    printf("Enter Staff ID to update: ");
    scanf("%d", &staff_id);
    getchar();
    
    Staff* staff = getStaff(staff_id);
    if (!staff) {
        printError("Staff not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Name [%s]: ", staff->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(staff->name, input);
    
    printf("Email [%s]: ", staff->email);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(staff->email, input);
    
    printf("Phone [%s]: ", staff->phone);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(staff->phone, input);
    
    if (updateStaff(*staff)) {
        printSuccess("Staff updated successfully!");
    } else {
        printError("Failed to update staff!");
    }
    
    free(staff);
    waitForKey();
}

void deleteStaffConsole() {
    printHeader("Delete Staff");
    
    int staff_id;
    printf("Enter Staff ID to delete: ");
    scanf("%d", &staff_id);
    getchar();
    
    if (deleteStaff(staff_id)) {
        printSuccess("Staff deleted successfully!");
    } else {
        printError("Failed to delete staff!");
    }
    
    waitForKey();
}

void viewStaffConsole() {
    printHeader("View Staff");
    
    int staff_id;
    printf("Enter Staff ID to view: ");
    scanf("%d", &staff_id);
    getchar();
    
    Staff* staff = getStaff(staff_id);
    if (!staff) {
        printError("Staff not found!");
        waitForKey();
        return;
    }
    
    printf("\nStaff Details:\n");
    printf("ID: %d\n", staff->staff_id);
    printf("Name: %s\n", staff->name);
    printf("Email: %s\n", staff->email);
    printf("Phone: %s\n", staff->phone);
    
    free(staff);
    waitForKey();
}

void listAllStaffConsole() {
    printHeader("All Staff");
    
    int count;
    Staff* staff = getAllStaff(&count);
    
    if (!staff) {
        printError("Failed to retrieve staff!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No staff found.\n");
    } else {
        printf("%-5s %-30s %-30s %-15s\n", "ID", "Name", "Email", "Phone");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = 0; i < count; i++) {
            printf("%-5d %-30s %-30s %-15s\n",
                   staff[i].staff_id,
                   staff[i].name,
                   staff[i].email,
                   staff[i].phone);
        }
    }
    
    free(staff);
    waitForKey();
}

// Borrowing menu and operations
void showBorrowingMenu() {
    int choice;
    do {
        printHeader("Borrowing Management");
        printf("1. Add Borrowing\n");
        printf("2. Update Borrowing\n");
        printf("3. Delete Borrowing\n");
        printf("4. View Borrowing\n");
        printf("5. List All Borrowings\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: addBorrowingConsole(); break;
            case 2: updateBorrowingConsole(); break;
            case 3: deleteBorrowingConsole(); break;
            case 4: viewBorrowingConsole(); break;
            case 5: listAllBorrowingsConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addBorrowingConsole() {
    printHeader("Add New Borrowing");
    
    Borrowing borrowing;
    printf("Enter borrowing details:\n");
    
    printf("Book ID: ");
    scanf("%d", &borrowing.book_id);
    getchar();
    
    printf("Member ID: ");
    scanf("%d", &borrowing.member_id);
    getchar();
    
    printf("Staff ID: ");
    scanf("%d", &borrowing.staff_id);
    getchar();
    
    char dateStr[20];
    printf("Borrow Date (YYYY-MM-DD): ");
    fgets(dateStr, sizeof(dateStr), stdin);
    dateStr[strcspn(dateStr, "\n")] = 0;
    borrowing.borrow_date = parseDate(dateStr);
    
    printf("Due Date (YYYY-MM-DD): ");
    fgets(dateStr, sizeof(dateStr), stdin);
    dateStr[strcspn(dateStr, "\n")] = 0;
    borrowing.due_date = parseDate(dateStr);
    
    borrowing.return_date = 0; // Initialize return date to 0 (not returned yet)
    
    if (addBorrowing(borrowing)) {
        printSuccess("Borrowing added successfully!");
    } else {
        printError("Failed to add borrowing!");
    }
    
    waitForKey();
}

void updateBorrowingConsole() {
    printHeader("Update Borrowing");
    
    int borrowing_id;
    printf("Enter Borrowing ID to update: ");
    scanf("%d", &borrowing_id);
    getchar();
    
    Borrowing* borrowing = getBorrowing(borrowing_id);
    if (!borrowing) {
        printError("Borrowing not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Book ID [%d]: ", borrowing->book_id);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) borrowing->book_id = atoi(input);
    
    printf("Member ID [%d]: ", borrowing->member_id);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) borrowing->member_id = atoi(input);
    
    printf("Staff ID [%d]: ", borrowing->staff_id);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) borrowing->staff_id = atoi(input);
    
    char dateStr[20];
    char formattedDate[20];
    formatDate(borrowing->borrow_date, formattedDate);
    printf("Borrow Date [%s]: ", formattedDate);
    fgets(dateStr, sizeof(dateStr), stdin);
    dateStr[strcspn(dateStr, "\n")] = 0;
    if (strlen(dateStr) > 0) borrowing->borrow_date = parseDate(dateStr);
    
    formatDate(borrowing->due_date, formattedDate);
    printf("Due Date [%s]: ", formattedDate);
    fgets(dateStr, sizeof(dateStr), stdin);
    dateStr[strcspn(dateStr, "\n")] = 0;
    if (strlen(dateStr) > 0) borrowing->due_date = parseDate(dateStr);
    
    if (borrowing->return_date != 0) {
        formatDate(borrowing->return_date, formattedDate);
        printf("Return Date [%s]: ", formattedDate);
    } else {
        printf("Return Date [Not returned]: ");
    }
    fgets(dateStr, sizeof(dateStr), stdin);
    dateStr[strcspn(dateStr, "\n")] = 0;
    if (strlen(dateStr) > 0) borrowing->return_date = parseDate(dateStr);
    
    if (updateBorrowing(*borrowing)) {
        printSuccess("Borrowing updated successfully!");
    } else {
        printError("Failed to update borrowing!");
    }
    
    free(borrowing);
    waitForKey();
}

void deleteBorrowingConsole() {
    printHeader("Delete Borrowing");
    
    int borrowing_id;
    printf("Enter Borrowing ID to delete: ");
    scanf("%d", &borrowing_id);
    getchar();
    
    if (deleteBorrowing(borrowing_id)) {
        printSuccess("Borrowing deleted successfully!");
    } else {
        printError("Failed to delete borrowing!");
    }
    
    waitForKey();
}

void viewBorrowingConsole() {
    printHeader("View Borrowing");
    
    int borrowing_id;
    printf("Enter Borrowing ID to view: ");
    scanf("%d", &borrowing_id);
    getchar();
    
    Borrowing* borrowing = getBorrowing(borrowing_id);
    if (!borrowing) {
        printError("Borrowing not found!");
        waitForKey();
        return;
    }
    
    char formattedDate[20];
    
    printf("\nBorrowing Details:\n");
    printf("ID: %d\n", borrowing->borrowing_id);
    printf("Book ID: %d\n", borrowing->book_id);
    printf("Member ID: %d\n", borrowing->member_id);
    printf("Staff ID: %d\n", borrowing->staff_id);
    
    formatDate(borrowing->borrow_date, formattedDate);
    printf("Borrow Date: %s\n", formattedDate);
    
    formatDate(borrowing->due_date, formattedDate);
    printf("Due Date: %s\n", formattedDate);
    
    if (borrowing->return_date != 0) {
        formatDate(borrowing->return_date, formattedDate);
        printf("Return Date: %s\n", formattedDate);
    } else {
        printf("Return Date: Not returned\n");
    }
    
    free(borrowing);
    waitForKey();
}

void listAllBorrowingsConsole() {
    printHeader("All Borrowings");
    
    int count;
    Borrowing* borrowings = getAllBorrowings(&count);
    
    if (!borrowings) {
        printError("Failed to retrieve borrowings!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No borrowings found.\n");
    } else {
        printf("%-5s %-8s %-8s %-8s %-12s %-12s %-12s\n", 
               "ID", "Book ID", "Member ID", "Staff ID", "Borrow Date", "Due Date", "Return Date");
        printf("----------------------------------------------------------------------\n");
        
        char formattedDate[20];
        for (int i = 0; i < count; i++) {
            formatDate(borrowings[i].borrow_date, formattedDate);
            char borrowDate[20];
            strcpy(borrowDate, formattedDate);
            
            formatDate(borrowings[i].due_date, formattedDate);
            char dueDate[20];
            strcpy(dueDate, formattedDate);
            
            char returnDate[20];
            if (borrowings[i].return_date != 0) {
                formatDate(borrowings[i].return_date, formattedDate);
                strcpy(returnDate, formattedDate);
            } else {
                strcpy(returnDate, "Not returned");
            }
            
            printf("%-5d %-8d %-8d %-8d %-12s %-12s %-12s\n",
                   borrowings[i].borrowing_id,
                   borrowings[i].book_id,
                   borrowings[i].member_id,
                   borrowings[i].staff_id,
                   borrowDate,
                   dueDate,
                   returnDate);
        }
    }
    
    free(borrowings);
    waitForKey();
}

// Fine menu and operations
void showFineMenu() {
    int choice;
    do {
        printHeader("Fine Management");
        printf("1. Add Fine\n");
        printf("2. Update Fine\n");
        printf("3. Delete Fine\n");
        printf("4. View Fine\n");
        printf("5. List All Fines\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice (1-6): ");
        
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1: addFineConsole(); break;
            case 2: updateFineConsole(); break;
            case 3: deleteFineConsole(); break;
            case 4: viewFineConsole(); break;
            case 5: listAllFinesConsole(); break;
            case 6: return;
            default:
                printError("Invalid choice!");
                waitForKey();
        }
    } while (1);
}

void addFineConsole() {
    printHeader("Add New Fine");
    
    Fine fine;
    printf("Enter fine details:\n");
    
    printf("Borrowing ID: ");
    scanf("%d", &fine.borrowing_id);
    getchar();
    
    printf("Amount: ");
    scanf("%lf", &fine.amount);
    getchar();
    
    fine.paid = 0; // Initialize as unpaid
    fine.date_paid = 0; // Initialize as not paid
    
    if (addFine(fine)) {
        printSuccess("Fine added successfully!");
    } else {
        printError("Failed to add fine!");
    }
    
    waitForKey();
}

void updateFineConsole() {
    printHeader("Update Fine");
    
    int fine_id;
    printf("Enter Fine ID to update: ");
    scanf("%d", &fine_id);
    getchar();
    
    Fine* fine = getFine(fine_id);
    if (!fine) {
        printError("Fine not found!");
        waitForKey();
        return;
    }
    
    printf("Enter new details (press Enter to keep current value):\n");
    
    char input[256];
    printf("Borrowing ID [%d]: ", fine->borrowing_id);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) fine->borrowing_id = atoi(input);
    
    printf("Amount [%.2f]: ", fine->amount);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) fine->amount = atof(input);
    
    printf("Paid [%s]: ", fine->paid ? "Yes" : "No");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        if (strcmp(input, "Yes") == 0 || strcmp(input, "yes") == 0 || strcmp(input, "Y") == 0 || strcmp(input, "y") == 0) {
            fine->paid = 1;
            if (fine->date_paid == 0) {
                fine->date_paid = time(NULL); // Set current time as payment date
            }
        } else {
            fine->paid = 0;
            fine->date_paid = 0;
        }
    }
    
    if (updateFine(*fine)) {
        printSuccess("Fine updated successfully!");
    } else {
        printError("Failed to update fine!");
    }
    
    free(fine);
    waitForKey();
}

void deleteFineConsole() {
    printHeader("Delete Fine");
    
    int fine_id;
    printf("Enter Fine ID to delete: ");
    scanf("%d", &fine_id);
    getchar();
    
    if (deleteFine(fine_id)) {
        printSuccess("Fine deleted successfully!");
    } else {
        printError("Failed to delete fine!");
    }
    
    waitForKey();
}

void viewFineConsole() {
    printHeader("View Fine");
    
    int fine_id;
    printf("Enter Fine ID to view: ");
    scanf("%d", &fine_id);
    getchar();
    
    Fine* fine = getFine(fine_id);
    if (!fine) {
        printError("Fine not found!");
        waitForKey();
        return;
    }
    
    char formattedDate[20];
    
    printf("\nFine Details:\n");
    printf("ID: %d\n", fine->fine_id);
    printf("Borrowing ID: %d\n", fine->borrowing_id);
    printf("Amount: %.2f\n", fine->amount);
    printf("Paid: %s\n", fine->paid ? "Yes" : "No");
    
    if (fine->paid && fine->date_paid != 0) {
        formatDate(fine->date_paid, formattedDate);
        printf("Date Paid: %s\n", formattedDate);
    } else {
        printf("Date Paid: Not paid yet\n");
    }
    
    free(fine);
    waitForKey();
}

void listAllFinesConsole() {
    printHeader("All Fines");
    
    int count;
    Fine* fines = getAllFines(&count);
    
    if (!fines) {
        printError("Failed to retrieve fines!");
        waitForKey();
        return;
    }
    
    if (count == 0) {
        printf("No fines found.\n");
    } else {
        printf("%-5s %-12s %-10s %-8s %-12s\n", "ID", "Borrowing ID", "Amount", "Paid", "Date Paid");
        printf("----------------------------------------------------------------------\n");
        
        char formattedDate[20];
        for (int i = 0; i < count; i++) {
            char datePaid[20];
            if (fines[i].paid && fines[i].date_paid != 0) {
                formatDate(fines[i].date_paid, formattedDate);
                strcpy(datePaid, formattedDate);
            } else {
                strcpy(datePaid, "Not paid");
            }
            
            printf("%-5d %-12d %-10.2f %-8s %-12s\n",
                   fines[i].fine_id,
                   fines[i].borrowing_id,
                   fines[i].amount,
                   fines[i].paid ? "Yes" : "No",
                   datePaid);
        }
    }
    
    free(fines);
    waitForKey();
}

// ... existing code ... 