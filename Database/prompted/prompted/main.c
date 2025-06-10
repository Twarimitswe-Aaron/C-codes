#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "gui.h"
#include "console.h"

// Function declarations
void showModeSelection();
void initializeDatabase();

int main() {
    // Initialize database directory and files
    initializeDatabase();
    
    // Show mode selection
    showModeSelection();
    
    return 0;
}

void showModeSelection() {
    int choice;
    system("cls");  // Clear screen
    
    printf("\n=== Library Management System ===\n");
    printf("Please select your preferred interface:\n");
    printf("1. Graphical User Interface (GUI)\n");
    printf("2. Console Interface\n");
    printf("3. Exit\n");
    printf("\nEnter your choice (1-3): ");
    
    scanf("%d", &choice);
    getchar(); // Clear input buffer
    
    switch(choice) {
        case 1:
            startGUI();
            break;
        case 2:
            startConsole();
            break;
        case 3:
            printf("\nThank you for using Library Management System!\n");
            exit(0);
        default:
            printf("\nInvalid choice! Please try again.\n");
            Sleep(1500);
            showModeSelection();
    }
}

void initializeDatabase() {
    // Create database directory if it doesn't exist
    CreateDirectory("database", NULL);
    
    // Initialize data files if they don't exist
    FILE* file;
    
    // Books
    file = fopen("database/books.dat", "ab");
    if (file) fclose(file);
    
    // Authors
    file = fopen("database/authors.dat", "ab");
    if (file) fclose(file);
    
    // Publishers
    file = fopen("database/publishers.dat", "ab");
    if (file) fclose(file);
    
    // Members
    file = fopen("database/members.dat", "ab");
    if (file) fclose(file);
    
    // Staff
    file = fopen("database/staff.dat", "ab");
    if (file) fclose(file);
    
    // Borrowings
    file = fopen("database/borrowings.dat", "ab");
    if (file) fclose(file);
    
    // Fines
    file = fopen("database/fines.dat", "ab");
    if (file) fclose(file);
} 