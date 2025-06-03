#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "book_manager.h"

void showConsoleMenu() {
    int choice;
    
    while(1) {
        system("cls");
        printf("\nLibrary Management System - Console Interface\n");
        printf("===========================================\n");
        printf("1. Book Management\n");
        printf("2. Author Management\n");
        printf("3. Publisher Management\n");
        printf("4. Member Management\n");
        printf("5. Staff Management\n");
        printf("6. Borrowing Management\n");
        printf("7. Fine Management\n");
        printf("8. Exit\n");
        printf("Enter your choice (1-8): ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                int bookChoice;
                do {
                    system("cls");
                    printf("\nBook Management\n");
                    printf("=============\n");
                    printf("1. Add Book\n");
                    printf("2. View All Books\n");
                    printf("3. Back to Main Menu\n");
                    printf("Enter your choice (1-3): ");
                    scanf("%d", &bookChoice);
                    getchar();
                    switch(bookChoice) {
                        case 1: addBookConsole(); break;
                        case 2: viewAllBooksConsole(); break;
                        case 3: break;
                        default: printf("Invalid choice!\n"); getchar();
                    }
                } while(bookChoice != 3);
                break;
            case 8:
                printf("Thank you for using the Library Management System!\n");
                return;
            default:
                printf("Invalid choice!\n"); getchar();
        }
    }
}