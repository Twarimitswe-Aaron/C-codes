#include "book_manager.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function definitions for Book Management (Console)

void addBookConsole() {
    Book newBook;
    // Implementation for adding a book from console input
    printf("\nEnter Book ID: ");
    scanf("%d", &newBook.book_id);
    printf("Enter Title: ");
    // Using fgets to avoid buffer overflow with spaces in title
    fgets(newBook.title, MAX_STRING, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove trailing newline

    printf("Enter Author ID: ");
    scanf("%d", &newBook.author_id);
    printf("Enter Publisher ID: ");
    scanf("%d", &newBook.publisher_id);
    printf("Enter ISBN: ");
     fgets(newBook.isbn, MAX_STRING, stdin);
    newBook.isbn[strcspn(newBook.isbn, "\n")] = 0;

    printf("Enter Genre: ");
     fgets(newBook.genre, MAX_STRING, stdin);
    newBook.genre[strcspn(newBook.genre, "\n")] = 0;

    printf("Enter Year Published: ");
    scanf("%d", &newBook.year_published);
    printf("Enter Copies Available: ");
    scanf("%d", &newBook.copies_available);
    printf("Enter Shelf Location: ");
     fgets(newBook.shelf_location, MAX_STRING, stdin);
    newBook.shelf_location[strcspn(newBook.shelf_location, "\n")] = 0;


    saveBook(newBook); // Save the new book using the database function
    printf("Book added successfully!\n");
}

void viewAllBooksConsole() {
    // Implementation for viewing all books in console
    int count;
    Book* books = loadBooks(&count); // Load books using the database function

    printf("\n--- All Books ---\n");
    if (count == 0) {
        printf("No books found.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("Book ID: %d, Title: %s, Author ID: %d, Publisher ID: %d, ISBN: %s, Genre: %s, Year: %d, Copies: %d, Shelf Location: %s\n",
                   books[i].book_id, books[i].title, books[i].author_id, books[i].publisher_id,
                   books[i].isbn, books[i].genre, books[i].year_published,
                   books[i].copies_available, books[i].shelf_location);
        }
        free(books); // Free allocated memory
    }
    printf("-----------------\n");
}

// The definition for showBookManagementGUI is in gui.c
// void showBookManagementGUI() is declared in book_manager.h