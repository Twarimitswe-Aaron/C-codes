#include "../include/database.h"
#include "../include/book.h"
#include "../include/author.h"
#include "../include/publisher.h"
#include "../include/member.h"
#include "../include/staff.h"
#include "../include/borrowing.h"
#include "../include/fine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void print_menu();
void handle_books(Database *db);
void handle_authors(Database *db);
void handle_publishers(Database *db);
void handle_members(Database *db);
void handle_staff(Database *db);
void handle_borrowings(Database *db);
void handle_fines(Database *db);
void db_disconnect(Database *db);

int main() {
    printf("Starting Library Management System...\n");
    
    // Initialize database connection
    printf("Initializing database...\n");
    Database *db = db_init();
    if (!db) {
        fprintf(stderr, "Failed to initialize database\n");
        return 1;
    }
    printf("Database initialized successfully.\n");

    printf("Connecting to database...\n");
    if (!db_connect(db)) {
        fprintf(stderr, "Failed to connect to database\n");
        db_close(db);
        return 1;
    }
    printf("Connected to database successfully.\n");

    int choice;
    do {
        print_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                handle_books(db);
                break;
            case 2:
                handle_authors(db);
                break;
            case 3:
                handle_publishers(db);
                break;
            case 4:
                handle_members(db);
                break;
            case 5:
                handle_staff(db);
                break;
            case 6:
                handle_borrowings(db);
                break;
            case 7:
                handle_fines(db);
                break;
            case 0:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    db_disconnect(db);
    return 0;
}

void print_menu() {
    printf("\nLibrary Management System\n");
    printf("1. Manage Books\n");
    printf("2. Manage Authors\n");
    printf("3. Manage Publishers\n");
    printf("4. Manage Members\n");
    printf("5. Manage Staff\n");
    printf("6. Manage Borrowings\n");
    printf("7. Manage Fines\n");
    printf("0. Exit\n");
}

void handle_books(Database *db) {
    int choice;
    do {
        printf("\nBook Management\n");
        printf("1. Add New Book\n");
        printf("2. List All Books\n");
        printf("3. Find Book by ID\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                char title[256], isbn[20], genre[50], shelf_location[50] = "A1";
                int author_id, publisher_id, year_published, copies_available = 1;

                printf("Enter title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;

                printf("Enter ISBN: ");
                fgets(isbn, sizeof(isbn), stdin);
                isbn[strcspn(isbn, "\n")] = 0;

                printf("Enter genre: ");
                fgets(genre, sizeof(genre), stdin);
                genre[strcspn(genre, "\n")] = 0;

                printf("Enter author ID: ");
                scanf("%d", &author_id);

                printf("Enter publisher ID: ");
                scanf("%d", &publisher_id);

                printf("Enter year published: ");
                scanf("%d", &year_published);

                // Optionally ask for copies and shelf location
                // printf("Enter copies available: ");
                // scanf("%d", &copies_available);
                // printf("Enter shelf location: ");
                // scanf("%s", shelf_location);

                Book *book = book_create(title, author_id, publisher_id, isbn, genre, year_published, copies_available, shelf_location);
                if (book != NULL) {
                    if (book_save(db, book)) {
                        printf("Book added successfully.\n");
                    } else {
                        printf("Failed to add book.\n");
                    }
                    book_free(book);
                }
                break;
            }
            case 2: {
                BookList *books = book_list_all(db);
                if (books != NULL) {
                    printf("\nAll Books:\n");
                    for (int i = 0; i < books->count; i++) {
                        printf("ID: %d, Title: %s, ISBN: %s\n",
                               books->books[i]->book_id,
                               books->books[i]->title,
                               books->books[i]->isbn);
                    }
                    book_list_free(books);
                }
                break;
            }
            case 3: {
                int book_id;
                printf("Enter book ID: ");
                scanf("%d", &book_id);

                Book *book = book_find_by_id(db, book_id);
                if (book != NULL) {
                    printf("\nBook Details:\n");
                    printf("ID: %d\n", book->book_id);
                    printf("Title: %s\n", book->title);
                    printf("ISBN: %s\n", book->isbn);
                    printf("Genre: %s\n", book->genre);
                    printf("Year Published: %d\n", book->year_published);
                    printf("Copies Available: %d\n", book->copies_available);
                    printf("Shelf Location: %s\n", book->shelf_location);
                    printf("Author ID: %d\n", book->author_id);
                    printf("Publisher ID: %d\n", book->publisher_id);
                    book_free(book);
                } else {
                    printf("Book not found.\n");
                }
                break;
            }
            case 4: {
                int book_id;
                printf("Enter book ID to update: ");
                scanf("%d", &book_id);
                getchar();

                Book *book = book_find_by_id(db, book_id);
                if (book != NULL) {
                    char title[256], isbn[20], genre[50], shelf_location[50];
                    int author_id, publisher_id, year_published, copies_available;

                    printf("Enter new title (or press Enter to keep current): ");
                    fgets(title, sizeof(title), stdin);
                    title[strcspn(title, "\n")] = 0;
                    if (strlen(title) > 0) {
                        strncpy(book->title, title, sizeof(book->title) - 1);
                    }

                    printf("Enter new ISBN (or press Enter to keep current): ");
                    fgets(isbn, sizeof(isbn), stdin);
                    isbn[strcspn(isbn, "\n")] = 0;
                    if (strlen(isbn) > 0) {
                        strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
                    }

                    printf("Enter new genre (or press Enter to keep current): ");
                    fgets(genre, sizeof(genre), stdin);
                    genre[strcspn(genre, "\n")] = 0;
                    if (strlen(genre) > 0) {
                        strncpy(book->genre, genre, sizeof(book->genre) - 1);
                    }

                    printf("Enter new author ID (or 0 to keep current): ");
                    scanf("%d", &author_id);
                    if (author_id > 0) {
                        book->author_id = author_id;
                    }

                    printf("Enter new publisher ID (or 0 to keep current): ");
                    scanf("%d", &publisher_id);
                    if (publisher_id > 0) {
                        book->publisher_id = publisher_id;
                    }

                    printf("Enter new year published (or 0 to keep current): ");
                    scanf("%d", &year_published);
                    if (year_published > 0) {
                        book->year_published = year_published;
                    }

                    printf("Enter new copies available (or 0 to keep current): ");
                    scanf("%d", &copies_available);
                    if (copies_available > 0) {
                        book->copies_available = copies_available;
                    }

                    printf("Enter new shelf location (or press Enter to keep current): ");
                    getchar();
                    fgets(shelf_location, sizeof(shelf_location), stdin);
                    shelf_location[strcspn(shelf_location, "\n")] = 0;
                    if (strlen(shelf_location) > 0) {
                        strncpy(book->shelf_location, shelf_location, sizeof(book->shelf_location) - 1);
                    }

                    if (book_update(db, book)) {
                        printf("Book updated successfully.\n");
                    } else {
                        printf("Failed to update book.\n");
                    }
                    book_free(book);
                } else {
                    printf("Book not found.\n");
                }
                break;
            }
            case 5: {
                int book_id;
                printf("Enter book ID to delete: ");
                scanf("%d", &book_id);

                if (book_delete(db, book_id)) {
                    printf("Book deleted successfully.\n");
                } else {
                    printf("Failed to delete book.\n");
                }
                break;
            }
        }
    } while (choice != 0);
}

void handle_authors(Database *db) {
    int choice;
    do {
        printf("\nAuthor Management\n");
        printf("1. Add New Author\n");
        printf("2. List All Authors\n");
        printf("3. Find Author by ID\n");
        printf("4. Update Author\n");
        printf("5. Delete Author\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                char name[100], bio[1000];
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter bio: ");
                fgets(bio, sizeof(bio), stdin);
                bio[strcspn(bio, "\n")] = 0;

                Author *author = author_create(name, bio);
                if (author != NULL) {
                    if (author_save(db, author)) {
                        printf("Author added successfully.\n");
                    } else {
                        printf("Failed to add author.\n");
                    }
                    author_free(author);
                }
                break;
            }
            case 2: {
                AuthorList *authors = author_list_all(db);
                if (authors != NULL) {
                    printf("\nAll Authors:\n");
                    for (int i = 0; i < authors->count; i++) {
                        printf("ID: %d, Name: %s\n",
                               authors->authors[i]->author_id,
                               authors->authors[i]->name);
                    }
                    author_list_free(authors);
                }
                break;
            }
            case 3: {
                int author_id;
                printf("Enter author ID: ");
                scanf("%d", &author_id);

                Author *author = author_find_by_id(db, author_id);
                if (author != NULL) {
                    printf("\nAuthor Details:\n");
                    printf("ID: %d\n", author->author_id);
                    printf("Name: %s\n", author->name);
                    printf("Bio: %s\n", author->bio);
                    author_free(author);
                } else {
                    printf("Author not found.\n");
                }
                break;
            }
            case 4: {
                int author_id;
                printf("Enter author ID to update: ");
                scanf("%d", &author_id);

                Author *author = author_find_by_id(db, author_id);
                if (author != NULL) {
                    char name[100], bio[1000];

                    printf("Enter new name (or press Enter to keep current): ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;
                    if (strlen(name) > 0) {
                        strncpy(author->name, name, sizeof(author->name) - 1);
                    }

                    printf("Enter new bio (or press Enter to keep current): ");
                    fgets(bio, sizeof(bio), stdin);
                    bio[strcspn(bio, "\n")] = 0;
                    if (strlen(bio) > 0) {
                        strncpy(author->bio, bio, sizeof(author->bio) - 1);
                    }

                    if (author_update(db, author)) {
                        printf("Author updated successfully.\n");
                    } else {
                        printf("Failed to update author.\n");
                    }
                    author_free(author);
                } else {
                    printf("Author not found.\n");
                }
                break;
            }
            case 5: {
                int author_id;
                printf("Enter author ID to delete: ");
                scanf("%d", &author_id);

                if (author_delete(db, author_id)) {
                    printf("Author deleted successfully.\n");
                } else {
                    printf("Failed to delete author.\n");
                }
                break;
            }
        }
    } while (choice != 0);
}

void handle_publishers(Database *db) {
    int choice;
    do {
        printf("\nPublisher Management\n");
        printf("1. Add New Publisher\n");
        printf("2. List All Publishers\n");
        printf("3. Find Publisher by ID\n");
        printf("4. Update Publisher\n");
        printf("5. Delete Publisher\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                char name[100], address[255], contact_info[100];
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter address: ");
                fgets(address, sizeof(address), stdin);
                address[strcspn(address, "\n")] = 0;

                printf("Enter contact info: ");
                fgets(contact_info, sizeof(contact_info), stdin);
                contact_info[strcspn(contact_info, "\n")] = 0;

                Publisher *publisher = publisher_create(name, address, contact_info);
                if (publisher != NULL) {
                    if (publisher_save(db, publisher)) {
                        printf("Publisher added successfully.\n");
                    } else {
                        printf("Failed to add publisher.\n");
                    }
                    publisher_free(publisher);
                }
                break;
            }
            case 2: {
                PublisherList *publishers = publisher_list_all(db);
                if (publishers != NULL) {
                    printf("\nAll Publishers:\n");
                    for (int i = 0; i < publishers->count; i++) {
                        printf("ID: %d, Name: %s\n",
                               publishers->publishers[i]->publisher_id,
                               publishers->publishers[i]->name);
                    }
                    publisher_list_free(publishers);
                }
                break;
            }
            case 3: {
                int publisher_id;
                printf("Enter publisher ID: ");
                scanf("%d", &publisher_id);

                Publisher *publisher = publisher_find_by_id(db, publisher_id);
                if (publisher != NULL) {
                    printf("\nPublisher Details:\n");
                    printf("ID: %d\n", publisher->publisher_id);
                    printf("Name: %s\n", publisher->name);
                    printf("Address: %s\n", publisher->address);
                    printf("Contact Info: %s\n", publisher->contact_info);
                    publisher_free(publisher);
                } else {
                    printf("Publisher not found.\n");
                }
                break;
            }
            case 4: {
                int publisher_id;
                printf("Enter publisher ID to update: ");
                scanf("%d", &publisher_id);

                Publisher *publisher = publisher_find_by_id(db, publisher_id);
                if (publisher != NULL) {
                    char name[100], address[255], contact_info[100];

                    printf("Enter new name (or press Enter to keep current): ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;
                    if (strlen(name) > 0) {
                        strncpy(publisher->name, name, sizeof(publisher->name) - 1);
                    }

                    printf("Enter new address (or press Enter to keep current): ");
                    fgets(address, sizeof(address), stdin);
                    address[strcspn(address, "\n")] = 0;
                    if (strlen(address) > 0) {
                        strncpy(publisher->address, address, sizeof(publisher->address) - 1);
                    }

                    printf("Enter new contact info (or press Enter to keep current): ");
                    fgets(contact_info, sizeof(contact_info), stdin);
                    contact_info[strcspn(contact_info, "\n")] = 0;
                    if (strlen(contact_info) > 0) {
                        strncpy(publisher->contact_info, contact_info, sizeof(publisher->contact_info) - 1);
                    }

                    if (publisher_update(db, publisher)) {
                        printf("Publisher updated successfully.\n");
                    } else {
                        printf("Failed to update publisher.\n");
                    }
                    publisher_free(publisher);
                } else {
                    printf("Publisher not found.\n");
                }
                break;
            }
            case 5: {
                int publisher_id;
                printf("Enter publisher ID to delete: ");
                scanf("%d", &publisher_id);

                if (publisher_delete(db, publisher_id)) {
                    printf("Publisher deleted successfully.\n");
                } else {
                    printf("Failed to delete publisher.\n");
                }
                break;
            }
        }
    } while (choice != 0);
}

void handle_members(Database *db) {
    int choice;
    do {
        printf("\nMember Management\n");
        printf("1. Add New Member\n");
        printf("2. List All Members\n");
        printf("3. Find Member by ID\n");
        printf("4. Update Member\n");
        printf("5. Delete Member\n");
        printf("6. List Members by Status\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                char name[100], address[255], phone[20], email[100], status[20];
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter address: ");
                fgets(address, sizeof(address), stdin);
                address[strcspn(address, "\n")] = 0;

                printf("Enter phone: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = 0;

                printf("Enter email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                printf("Enter membership status (active/suspended): ");
                fgets(status, sizeof(status), stdin);
                status[strcspn(status, "\n")] = 0;

                Member *member = member_create(name, address, phone, email, time(NULL), status);
                if (member != NULL) {
                    if (member_save(db, member)) {
                        printf("Member added successfully.\n");
                    } else {
                        printf("Failed to add member.\n");
                    }
                    member_free(member);
                }
                break;
            }
            case 2: {
                MemberList *members = member_list_all(db);
                if (members != NULL) {
                    printf("\nAll Members:\n");
                    for (int i = 0; i < members->count; i++) {
                        printf("ID: %d, Name: %s, Status: %s\n",
                               members->members[i]->member_id,
                               members->members[i]->name,
                               members->members[i]->membership_status);
                    }
                    member_list_free(members);
                }
                break;
            }
            case 3: {
                int member_id;
                printf("Enter member ID: ");
                scanf("%d", &member_id);

                Member *member = member_find_by_id(db, member_id);
                if (member != NULL) {
                    printf("\nMember Details:\n");
                    printf("ID: %d\n", member->member_id);
                    printf("Name: %s\n", member->name);
                    printf("Address: %s\n", member->address);
                    printf("Phone: %s\n", member->phone);
                    printf("Email: %s\n", member->email);
                    printf("Status: %s\n", member->membership_status);
                    member_free(member);
                } else {
                    printf("Member not found.\n");
                }
                break;
            }
            case 4: {
                int member_id;
                printf("Enter member ID to update: ");
                scanf("%d", &member_id);

                Member *member = member_find_by_id(db, member_id);
                if (member != NULL) {
                    char name[100], address[255], phone[20], email[100], status[20];

                    printf("Enter new name (or press Enter to keep current): ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;
                    if (strlen(name) > 0) {
                        strncpy(member->name, name, sizeof(member->name) - 1);
                    }

                    printf("Enter new address (or press Enter to keep current): ");
                    fgets(address, sizeof(address), stdin);
                    address[strcspn(address, "\n")] = 0;
                    if (strlen(address) > 0) {
                        strncpy(member->address, address, sizeof(member->address) - 1);
                    }

                    printf("Enter new phone (or press Enter to keep current): ");
                    fgets(phone, sizeof(phone), stdin);
                    phone[strcspn(phone, "\n")] = 0;
                    if (strlen(phone) > 0) {
                        strncpy(member->phone, phone, sizeof(member->phone) - 1);
                    }

                    printf("Enter new email (or press Enter to keep current): ");
                    fgets(email, sizeof(email), stdin);
                    email[strcspn(email, "\n")] = 0;
                    if (strlen(email) > 0) {
                        strncpy(member->email, email, sizeof(member->email) - 1);
                    }

                    printf("Enter new status (or press Enter to keep current): ");
                    fgets(status, sizeof(status), stdin);
                    status[strcspn(status, "\n")] = 0;
                    if (strlen(status) > 0) {
                        strncpy(member->membership_status, status, sizeof(member->membership_status) - 1);
                    }

                    if (member_update(db, member)) {
                        printf("Member updated successfully.\n");
                    } else {
                        printf("Failed to update member.\n");
                    }
                    member_free(member);
                } else {
                    printf("Member not found.\n");
                }
                break;
            }
            case 5: {
                int member_id;
                printf("Enter member ID to delete: ");
                scanf("%d", &member_id);

                if (member_delete(db, member_id)) {
                    printf("Member deleted successfully.\n");
                } else {
                    printf("Failed to delete member.\n");
                }
                break;
            }
            case 6: {
                char status[20];
                printf("Enter status to filter (active/suspended): ");
                fgets(status, sizeof(status), stdin);
                status[strcspn(status, "\n")] = 0;

                MemberList *members = member_list_by_status(db, status);
                if (members != NULL) {
                    printf("\nMembers with status '%s':\n", status);
                    for (int i = 0; i < members->count; i++) {
                        printf("ID: %d, Name: %s\n",
                               members->members[i]->member_id,
                               members->members[i]->name);
                    }
                    member_list_free(members);
                }
                break;
            }
        }
    } while (choice != 0);
}

void handle_staff(Database *db) {
    int choice;
    do {
        printf("\nStaff Management\n");
        printf("1. Add New Staff\n");
        printf("2. List All Staff\n");
        printf("3. Find Staff by ID\n");
        printf("4. Update Staff\n");
        printf("5. Delete Staff\n");
        printf("6. List Staff by Role\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                char name[100], role[50], email[100], phone[20];
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter role: ");
                fgets(role, sizeof(role), stdin);
                role[strcspn(role, "\n")] = 0;

                printf("Enter email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                printf("Enter phone: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = 0;

                Staff *staff = staff_create(name, role, email, phone);
                if (staff != NULL) {
                    if (staff_save(db, staff)) {
                        printf("Staff added successfully.\n");
                    } else {
                        printf("Failed to add staff.\n");
                    }
                    staff_free(staff);
                }
                break;
            }
            case 2: {
                StaffList *staff_list = staff_list_all(db);
                if (staff_list != NULL) {
                    printf("\nAll Staff:\n");
                    for (int i = 0; i < staff_list->count; i++) {
                        printf("ID: %d, Name: %s, Role: %s\n",
                               staff_list->staff_members[i]->staff_id,
                               staff_list->staff_members[i]->name,
                               staff_list->staff_members[i]->role);
                    }
                    staff_list_free(staff_list);
                }
                break;
            }
            case 3: {
                int staff_id;
                printf("Enter staff ID: ");
                scanf("%d", &staff_id);

                Staff *staff = staff_find_by_id(db, staff_id);
                if (staff != NULL) {
                    printf("\nStaff Details:\n");
                    printf("ID: %d\n", staff->staff_id);
                    printf("Name: %s\n", staff->name);
                    printf("Role: %s\n", staff->role);
                    printf("Email: %s\n", staff->email);
                    printf("Phone: %s\n", staff->phone);
                    staff_free(staff);
                } else {
                    printf("Staff not found.\n");
                }
                break;
            }
            case 4: {
                int staff_id;
                printf("Enter staff ID to update: ");
                scanf("%d", &staff_id);

                Staff *staff = staff_find_by_id(db, staff_id);
                if (staff != NULL) {
                    char name[100], role[50], email[100], phone[20];

                    printf("Enter new name (or press Enter to keep current): ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;
                    if (strlen(name) > 0) {
                        strncpy(staff->name, name, sizeof(staff->name) - 1);
                    }

                    printf("Enter new role (or press Enter to keep current): ");
                    fgets(role, sizeof(role), stdin);
                    role[strcspn(role, "\n")] = 0;
                    if (strlen(role) > 0) {
                        strncpy(staff->role, role, sizeof(staff->role) - 1);
                    }

                    printf("Enter new email (or press Enter to keep current): ");
                    fgets(email, sizeof(email), stdin);
                    email[strcspn(email, "\n")] = 0;
                    if (strlen(email) > 0) {
                        strncpy(staff->email, email, sizeof(staff->email) - 1);
                    }

                    printf("Enter new phone (or press Enter to keep current): ");
                    fgets(phone, sizeof(phone), stdin);
                    phone[strcspn(phone, "\n")] = 0;
                    if (strlen(phone) > 0) {
                        strncpy(staff->phone, phone, sizeof(staff->phone) - 1);
                    }

                    if (staff_update(db, staff)) {
                        printf("Staff updated successfully.\n");
                    } else {
                        printf("Failed to update staff.\n");
                    }
                    staff_free(staff);
                } else {
                    printf("Staff not found.\n");
                }
                break;
            }
            case 5: {
                int staff_id;
                printf("Enter staff ID to delete: ");
                scanf("%d", &staff_id);

                if (staff_delete(db, staff_id)) {
                    printf("Staff deleted successfully.\n");
                } else {
                    printf("Failed to delete staff.\n");
                }
                break;
            }
            case 6: {
                char role[50];
                printf("Enter role to filter: ");
                fgets(role, sizeof(role), stdin);
                role[strcspn(role, "\n")] = 0;

                StaffList *staff_list = staff_list_by_role(db, role);
                if (staff_list != NULL) {
                    printf("\nStaff with role '%s':\n", role);
                    for (int i = 0; i < staff_list->count; i++) {
                        printf("ID: %d, Name: %s\n",
                               staff_list->staff_members[i]->staff_id,
                               staff_list->staff_members[i]->name);
                    }
                    staff_list_free(staff_list);
                }
                break;
            }
        }
    } while (choice != 0);
}

void handle_borrowings(Database *db) {
    int choice;
    do {
        printf("\nBorrowing Management\n");
        printf("1. Add New Borrowing\n");
        printf("2. List All Borrowings\n");
        printf("3. Find Borrowing by ID\n");
        printf("4. Update Borrowing\n");
        printf("5. Delete Borrowing\n");
        printf("6. List Borrowings by Book\n");
        printf("7. List Borrowings by Member\n");
        printf("8. List Borrowings by Status\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                int book_id, member_id, staff_id;
                time_t borrow_date, due_date;

                printf("Enter book ID: ");
                scanf("%d", &book_id);

                printf("Enter member ID: ");
                scanf("%d", &member_id);

                printf("Enter staff ID: ");
                scanf("%d", &staff_id);

                borrow_date = time(NULL);
                due_date = borrow_date + (14 * 24 * 60 * 60); // 14 days from now

                Borrowing *borrowing = borrowing_create(book_id, member_id, staff_id, borrow_date, due_date);
                if (borrowing != NULL) {
                    if (borrowing_save(db, borrowing)) {
                        printf("Borrowing added successfully.\n");
                    } else {
                        printf("Failed to add borrowing.\n");
                    }
                    borrowing_free(borrowing);
                }
                break;
            }
            case 2: {
                BorrowingList *borrowings = borrowing_list_all(db);
                if (borrowings != NULL) {
                    printf("\nAll Borrowings:\n");
                    for (int i = 0; i < borrowings->count; i++) {
                        printf("ID: %d, Book ID: %d, Member ID: %d, Status: %d\n",
                               borrowings->borrowings[i]->borrowing_id,
                               borrowings->borrowings[i]->book_id,
                               borrowings->borrowings[i]->member_id,
                               borrowings->borrowings[i]->status);
                    }
                    borrowing_list_free(borrowings);
                }
                break;
            }
            case 3: {
                int borrowing_id;
                printf("Enter borrowing ID: ");
                scanf("%d", &borrowing_id);

                Borrowing *borrowing = borrowing_find_by_id(db, borrowing_id);
                if (borrowing != NULL) {
                    printf("\nBorrowing Details:\n");
                    printf("ID: %d\n", borrowing->borrowing_id);
                    printf("Book ID: %d\n", borrowing->book_id);
                    printf("Member ID: %d\n", borrowing->member_id);
                    printf("Staff ID: %d\n", borrowing->staff_id);
                    printf("Status: %d\n", borrowing->status);
                    borrowing_free(borrowing);
                } else {
                    printf("Borrowing not found.\n");
                }
                break;
            }
            case 4: {
                int borrowing_id;
                printf("Enter borrowing ID to update: ");
                scanf("%d", &borrowing_id);

                Borrowing *borrowing = borrowing_find_by_id(db, borrowing_id);
                if (borrowing != NULL) {
                    int status;
                    printf("Enter new status (0=Borrowed, 1=Returned, 2=Overdue): ");
                    scanf("%d", &status);
                    borrowing->status = status;

                    if (borrowing_update(db, borrowing)) {
                        printf("Borrowing updated successfully.\n");
                    } else {
                        printf("Failed to update borrowing.\n");
                    }
                    borrowing_free(borrowing);
                } else {
                    printf("Borrowing not found.\n");
                }
                break;
            }
            case 5: {
                int borrowing_id;
                printf("Enter borrowing ID to delete: ");
                scanf("%d", &borrowing_id);

                if (borrowing_delete(db, borrowing_id)) {
                    printf("Borrowing deleted successfully.\n");
                } else {
                    printf("Failed to delete borrowing.\n");
                }
                break;
            }
            case 6: {
                int book_id;
                printf("Enter book ID: ");
                scanf("%d", &book_id);

                BorrowingList *borrowings = borrowing_list_by_book(db, book_id);
                if (borrowings != NULL) {
                    printf("\nBorrowings for Book ID %d:\n", book_id);
                    for (int i = 0; i < borrowings->count; i++) {
                        printf("ID: %d, Member ID: %d, Status: %d\n",
                               borrowings->borrowings[i]->borrowing_id,
                               borrowings->borrowings[i]->member_id,
                               borrowings->borrowings[i]->status);
                    }
                    borrowing_list_free(borrowings);
                }
                break;
            }
            case 7: {
                int member_id;
                printf("Enter member ID: ");
                scanf("%d", &member_id);

                BorrowingList *borrowings = borrowing_list_by_member(db, member_id);
                if (borrowings != NULL) {
                    printf("\nBorrowings for Member ID %d:\n", member_id);
                    for (int i = 0; i < borrowings->count; i++) {
                        printf("ID: %d, Book ID: %d, Status: %d\n",
                               borrowings->borrowings[i]->borrowing_id,
                               borrowings->borrowings[i]->book_id,
                               borrowings->borrowings[i]->status);
                    }
                    borrowing_list_free(borrowings);
                }
                break;
            }
            case 8: {
                int status;
                printf("Enter status (0=Borrowed, 1=Returned, 2=Overdue): ");
                scanf("%d", &status);

                BorrowingList *borrowings = borrowing_list_by_status(db, status);
                if (borrowings != NULL) {
                    printf("\nBorrowings with Status %d:\n", status);
                    for (int i = 0; i < borrowings->count; i++) {
                        printf("ID: %d, Book ID: %d, Member ID: %d\n",
                               borrowings->borrowings[i]->borrowing_id,
                               borrowings->borrowings[i]->book_id,
                               borrowings->borrowings[i]->member_id);
                    }
                    borrowing_list_free(borrowings);
                }
                break;
            }
        }
    } while (choice != 0);
}

void handle_fines(Database *db) {
    int choice;
    do {
        printf("\nFine Management\n");
        printf("1. Add New Fine\n");
        printf("2. List All Fines\n");
        printf("3. Find Fine by ID\n");
        printf("4. Update Fine\n");
        printf("5. Delete Fine\n");
        printf("6. List Fines by Borrowing\n");
        printf("7. List Fines by Status\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: {
                int borrowing_id;
                double amount;
                time_t due_date;

                printf("Enter borrowing ID: ");
                scanf("%d", &borrowing_id);

                printf("Enter amount: ");
                scanf("%lf", &amount);

                due_date = time(NULL) + (7 * 24 * 60 * 60); // 7 days from now

                Fine *fine = fine_create(borrowing_id, amount, due_date);
                if (fine != NULL) {
                    if (fine_save(db, fine)) {
                        printf("Fine added successfully.\n");
                    } else {
                        printf("Failed to add fine.\n");
                    }
                    fine_free(fine);
                }
                break;
            }
            case 2: {
                FineList *fines = fine_list_all(db);
                if (fines != NULL) {
                    printf("\nAll Fines:\n");
                    for (int i = 0; i < fines->count; i++) {
                        printf("ID: %d, Borrowing ID: %d, Amount: %.2f, Status: %d\n",
                               fines->fines[i]->fine_id,
                               fines->fines[i]->borrowing_id,
                               fines->fines[i]->amount,
                               fines->fines[i]->status);
                    }
                    fine_list_free(fines);
                }
                break;
            }
            case 3: {
                int fine_id;
                printf("Enter fine ID: ");
                scanf("%d", &fine_id);

                Fine *fine = fine_find_by_id(db, fine_id);
                if (fine != NULL) {
                    printf("\nFine Details:\n");
                    printf("ID: %d\n", fine->fine_id);
                    printf("Borrowing ID: %d\n", fine->borrowing_id);
                    printf("Amount: %.2f\n", fine->amount);
                    printf("Status: %d\n", fine->status);
                    fine_free(fine);
                } else {
                    printf("Fine not found.\n");
                }
                break;
            }
            case 4: {
                int fine_id;
                printf("Enter fine ID to update: ");
                scanf("%d", &fine_id);

                Fine *fine = fine_find_by_id(db, fine_id);
                if (fine != NULL) {
                    int status;
                    printf("Enter new status (0=Pending, 1=Paid, 2=Waived): ");
                    scanf("%d", &status);
                    fine->status = status;

                    if (fine_update(db, fine)) {
                        printf("Fine updated successfully.\n");
                    } else {
                        printf("Failed to update fine.\n");
                    }
                    fine_free(fine);
                } else {
                    printf("Fine not found.\n");
                }
                break;
            }
            case 5: {
                int fine_id;
                printf("Enter fine ID to delete: ");
                scanf("%d", &fine_id);

                if (fine_delete(db, fine_id)) {
                    printf("Fine deleted successfully.\n");
                } else {
                    printf("Failed to delete fine.\n");
                }
                break;
            }
            case 6: {
                int borrowing_id;
                printf("Enter borrowing ID: ");
                scanf("%d", &borrowing_id);

                FineList *fines = fine_list_by_borrowing(db, borrowing_id);
                if (fines != NULL) {
                    printf("\nFines for Borrowing ID %d:\n", borrowing_id);
                    for (int i = 0; i < fines->count; i++) {
                        printf("ID: %d, Amount: %.2f, Status: %d\n",
                               fines->fines[i]->fine_id,
                               fines->fines[i]->amount,
                               fines->fines[i]->status);
                    }
                    fine_list_free(fines);
                }
                break;
            }
            case 7: {
                int status;
                printf("Enter status (0=Pending, 1=Paid, 2=Waived): ");
                scanf("%d", &status);

                FineList *fines = fine_list_by_status(db, status);
                if (fines != NULL) {
                    printf("\nFines with Status %d:\n", status);
                    for (int i = 0; i < fines->count; i++) {
                        printf("ID: %d, Borrowing ID: %d, Amount: %.2f\n",
                               fines->fines[i]->fine_id,
                               fines->fines[i]->borrowing_id,
                               fines->fines[i]->amount);
                    }
                    fine_list_free(fines);
                }
                break;
            }
        }
    } while (choice != 0);
} 