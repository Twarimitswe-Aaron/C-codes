#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "interface.h"

#include "manager.h"

void showConsoleMenu() {
    int choice;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (1) {
        system("cls");

        SetConsoleTextAttribute(hConsole, 11);
        printf("\n====================================================\n");
        printf("       <<< Library Management System >>>\n");
        printf("====================================================\n");

        SetConsoleTextAttribute(hConsole, 15);
        printf("\n> Main Menu:\n");
        printf("  ---------------------------------------\n");
        SetConsoleTextAttribute(hConsole, 10);
        printf("  [1] Book Management\n");
        SetConsoleTextAttribute(hConsole, 13);
        printf("  [2] Author Management\n");
        printf("  [3] Publisher Management\n");
        printf("  [4] Member Management\n");
        printf("  [5] Staff Management\n");
        printf("  [6] Borrowing Management\n");
        printf("  [7] Fine Management\n");
        SetConsoleTextAttribute(hConsole, 12);
        printf("  [8] Exit\n");
        SetConsoleTextAttribute(hConsole, 15);
        printf("  ---------------------------------------\n");
        printf(">> Enter your choice (1-8): ");

        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                bookManagementMenu();
                break;
            case 2:
                authorManagementMenu();
                break;
            case 3:
                publisherManagementMenu();
                break;
            case 4:
                memberManagementMenu();
                break;
            case 5:
                staffManagementMenu();
                break;
            case 6:
                borrowingManagementMenu();
                break;
            case 7:
                fineManagementMenu();
                break;
            case 8:
                SetConsoleTextAttribute(hConsole, 10);
                printf("\n>> Thank you for using the Library Management System!\n");
                SetConsoleTextAttribute(hConsole, 7);
                return;
            default:
                SetConsoleTextAttribute(hConsole, 12);
                printf("!! Invalid choice!\n");
                SetConsoleTextAttribute(hConsole, 7);
                system("pause");
        }
    }
}
