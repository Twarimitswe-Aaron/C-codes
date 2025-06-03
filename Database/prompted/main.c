#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "interface.h"

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main() {
    int choice;

    system("cls");

    setConsoleColor(11);
    printf("====================================================\n");
    setConsoleColor(14);
    printf("         >>> Welcome to the Library System <<<       \n");
    setConsoleColor(11);
    printf("====================================================\n");

    setConsoleColor(10);
    printf("\n> Choose your interface:\n");
    setConsoleColor(15);
    printf("  ------------------------------\n");
    setConsoleColor(13);
    printf("  [1] Console Interface\n");
    printf("  [2] GUI Interface\n");
    setConsoleColor(15);
    printf("  ------------------------------\n");

    setConsoleColor(14);
    printf(">> Enter your choice (1-2): ");
    setConsoleColor(7);
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            showConsoleMenu();
            break;
        case 2: {
            HINSTANCE hInstance = GetModuleHandle(NULL);
            LPSTR lpCmdLine = GetCommandLineA();
            int nCmdShow = SW_SHOWDEFAULT;
            WinMain(hInstance, NULL, lpCmdLine, nCmdShow);
            break;
        }
        default:
            setConsoleColor(12);
            printf("\n!! Invalid choice. Please restart the program.\n");
            setConsoleColor(7);
            return 1;
    }

    setConsoleColor(7);
    return 0;
}
