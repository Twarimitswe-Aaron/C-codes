#include <stdio.h>

int main() {
    int n = 5;  // Diamond height (half)
    int totalRows = 2 * n - 1;  // Full diamond height

    for (int i = 0; i < totalRows; i++) {
        int spaces = (i < n) ? n - i - 1 : i - n + 1;  // Adjusted space logic
        int stars = 2 * (n - spaces) - 1;  // Corrected star formula

        // Print spaces
        for (int j = 0; j < spaces; j++) {
            printf(" ");
        }
        // Print stars
        for (int k = 0; k < stars; k++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}

