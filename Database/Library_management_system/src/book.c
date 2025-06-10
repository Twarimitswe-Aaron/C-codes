#include "book.h"
#include <stdlib.h>
#include <string.h>

Book* book_new(const char* title, const char* author, const char* publisher, const char* isbn) {
    Book* book = (Book*)malloc(sizeof(Book));
    if (book) {
        strncpy(book->title, title, sizeof(book->title) - 1);
        book->title[sizeof(book->title) - 1] = '\0';
        // Assuming author and publisher are IDs, you might need to look them up
        book->author_id = 0; // Replace with actual author ID lookup
        book->publisher_id = 0; // Replace with actual publisher ID lookup
        strncpy(book->isbn, isbn, sizeof(book->isbn) - 1);
        book->isbn[sizeof(book->isbn) - 1] = '\0';
    }
    return book;
} 