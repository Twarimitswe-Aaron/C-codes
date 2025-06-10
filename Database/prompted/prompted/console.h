#ifndef CONSOLE_H
#define CONSOLE_H

#include "structures.h"

// Main console functions
void startConsole();
void showConsoleMainMenu();
void showBookMenu();
void showAuthorMenu();
void showPublisherMenu();
void showMemberMenu();
void showStaffMenu();
void showBorrowingMenu();
void showFineMenu();

// Book operations
void addBookConsole();
void updateBookConsole();
void deleteBookConsole();
void viewBookConsole();
void listAllBooksConsole();

// Author operations
void addAuthorConsole();
void updateAuthorConsole();
void deleteAuthorConsole();
void viewAuthorConsole();
void listAllAuthorsConsole();

// Publisher operations
void addPublisherConsole();
void updatePublisherConsole();
void deletePublisherConsole();
void viewPublisherConsole();
void listAllPublishersConsole();

// Member operations
void addMemberConsole();
void updateMemberConsole();
void deleteMemberConsole();
void viewMemberConsole();
void listAllMembersConsole();

// Staff operations
void addStaffConsole();
void updateStaffConsole();
void deleteStaffConsole();
void viewStaffConsole();
void listAllStaffConsole();

// Borrowing operations
void addBorrowingConsole();
void updateBorrowingConsole();
void deleteBorrowingConsole();
void viewBorrowingConsole();
void listAllBorrowingsConsole();

// Fine operations
void addFineConsole();
void updateFineConsole();
void deleteFineConsole();
void viewFineConsole();
void listAllFinesConsole();

// Helper functions
void clearScreen();
void waitForKey();
void printHeader(const char* title);
void printError(const char* message);
void printSuccess(const char* message);

#endif // CONSOLE_H 