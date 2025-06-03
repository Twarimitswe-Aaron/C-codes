#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Function declarations
void showConsoleMenu();
void showGUIMenu();

int main() {
    int choice;
    
    system("cls");  // Clear screen
    printf("Library Management System\n");
    printf("========================\n");
    printf("1. Console Interface\n");
    printf("2. GUI Interface\n");
    printf("Enter your choice (1-2): ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            showConsoleMenu();
            break;
        case 2:
            // Call WinMain for GUI
            // Get the instance handle and command line
            HINSTANCE hInstance = GetModuleHandle(NULL);
            LPSTR lpCmdLine = GetCommandLineA();
            // Show the main window (nCmdShow parameter)
            int nCmdShow = SW_SHOWDEFAULT; // Or SW_SHOW
            WinMain(hInstance, NULL, lpCmdLine, nCmdShow);
            break;
        default:
            printf("Invalid choice!\n");
            return 1;
    }
    
    return 0;
}