#ifndef BOOK_MANAGER_H
#define BOOK_MANAGER_H

#include "database.h" // Include database.h for structure definitions and MAX_STRING/MAX_TEXT

// Function declarations for Book Management (Console)
void addBookConsole();
void viewAllBooksConsole();

// Function declarations for Book Management (GUI) - defined in gui.c
void showBookManagementGUI();

#endif // BOOK_MANAGER_H