#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "manager.h"

#define MAX_BOOKS 100

// -------- Book Management --------
typedef struct {
    int id;
    char title[100];
    char author[100];
} Book;

Book books[MAX_BOOKS];
int bookCount = 0;

HANDLE hConsole;

// Utility
void pause() {
    printf("\n>> Press any key to continue...\n");
    system("pause");
}

// Add Book
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("!! Book limit reached!\n");
        return;
    }

    Book b;
    b.id = bookCount + 1;

    printf(">> Enter book title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0;

    printf(">> Enter author name: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    books[bookCount++] = b;

    printf(">> Book added successfully with ID: %d\n", b.id);
    pause();
}

// View Books
void viewBooks() {
    if (bookCount == 0) {
        printf(">> No books found.\n");
    } else {
        printf(">> Books List:\n");
        for (int i = 0; i < bookCount; i++) {
            printf("  [%d] %s by %s\n", books[i].id, books[i].title, books[i].author);
        }
    }
    pause();
}

// Update Book
void updateBook() {
    int id;
    printf(">> Enter book ID to update: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            printf(">> Updating Book ID %d:\n", id);
            printf(">> New title: ");
            fgets(books[i].title, sizeof(books[i].title), stdin);
            books[i].title[strcspn(books[i].title, "\n")] = 0;

            printf(">> New author: ");
            fgets(books[i].author, sizeof(books[i].author), stdin);
            books[i].author[strcspn(books[i].author, "\n")] = 0;

            printf(">> Book updated successfully.\n");
            pause();
            return;
        }
    }
    printf("!! Book ID not found.\n");
    pause();
}

// Delete Book
void deleteBook() {
    int id;
    printf(">> Enter book ID to delete: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            for (int j = i; j < bookCount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf(">> Book ID %d deleted.\n", id);
            pause();
            return;
        }
    }
    printf("!! Book ID not found.\n");
    pause();
}

// Book Menu
void bookManagementMenu() {
    int choice;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    do {
        system("cls");
        SetConsoleTextAttribute(hConsole, 14);
        printf("\n========== Book Management ==========\n");
        SetConsoleTextAttribute(hConsole, 15);
        printf("  [1] Add Book\n");
        printf("  [2] View All Books\n");
        printf("  [3] Update Book\n");
        printf("  [4] Delete Book\n");
        printf("  [5] Back to Main Menu\n");
        printf(">> Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: updateBook(); break;
            case 4: deleteBook(); break;
            case 5: break;
            default:
                SetConsoleTextAttribute(hConsole, 12);
                printf("!! Invalid choice!\n");
                SetConsoleTextAttribute(hConsole, 7);
                pause();
        }
    } while (choice != 5);
}

////////////////////////////////////////////////////////
// The following are stub implementations for other menus
////////////////////////////////////////////////////////

void authorManagementMenu() {
    system("cls");
    printf(">> Author Management (Demo)\n");
    pause();
}

void publisherManagementMenu() {
    system("cls");
    printf(">> Publisher Management (Demo)\n");
    pause();
}

void memberManagementMenu() {
    system("cls");
    printf(">> Member Management (Demo)\n");
    pause();
}

void staffManagementMenu() {
    system("cls");
    printf(">> Staff Management (Demo)\n");
    pause();
}

void borrowingManagementMenu() {
    system("cls");
    printf(">> Borrowing Management (Demo)\n");
    pause();
}

void fineManagementMenu() {
    system("cls");
    printf(">> Fine Management (Demo)\n");
    pause();
}
