#include "./include/author.h"
#include "./include/database.h"

void handle_authors(Database *db);
void run_console_interface(Database *db);
void print_interface_menu();
void print_menu();

int main(){
    printf("Library management system intitialization ..\n");
    printf("Initializing Database...\n");
    Database *db=db_init();
    if(!db){
        fprintf(stderr, "Failed to initialize database..\n");
        return 1;
    }
    printf("Database initialized successfuly");
   
     printf("Connecting to database...\n");
    if (!db_connect(db)) {
        fprintf(stderr, "Failed to connect to database\n");
        db_close(db);
        return 1;
    }
    printf("Connected to database successfully.\n");

    int choice;
    do{
        printf("Enter your choice");
        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1:
            run_console_interface(db);

        }
    } while(choice !=0);
    db_close(db);
    return 0;
}

void print_interface_menu() {
    printf("\nLibrary Management System - Interface Selection\n");
    printf("1. Console Interface\n");
    printf("0. Exit\n");
}

void run_console_interface(Database *db){
    int choice;
    do{
        print_menu();
        printf("Enter your choice:");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            handle_authors(db);
            break;
        case 0:
           break;
        default:
            printf("Invalid choice. Please try again.\n");
        }


    }while (choice !=0);
}

void print_menu(){
    printf("\nLibrary Management System\n");
    printf("1. Manage Authors\n");

}

void handle_authors(Database *db){
    int choice;
    do{
        printf("\nAuthor Management\n");
        printf("1. Add New Author\n");
        printf("2. List All Authors\n");
        printf("3. Find Author by ID\n");
        printf("4. Update Author\n");
        printf("5. Delete Author\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1:{
                char name[50], email[50];
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("Enter email: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = 0;

                Author *author = author_create(name, email);
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
                    printf("email: %s\n", author->email);
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
                    char name[50], email[50];

                    printf("Enter new name (or press Enter to keep current): ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = 0;
                    if (strlen(name) > 0) {
                        strncpy(author->name, name, sizeof(author->name) - 1);
                    }

                    printf("Enter new email (or press Enter to keep current): ");
                    fgets(email, sizeof(email), stdin);
                    email[strcspn(email, "\n")] = 0;
                    if (strlen(email) > 0) {
                        strncpy(author->email, email, sizeof(author->email) - 1);
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
    }while (choice != 0);
}