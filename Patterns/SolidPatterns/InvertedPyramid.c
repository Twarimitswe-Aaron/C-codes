#include <stdio.h>

int main() {
    int n = 5;  // Number of rows

    for (int i = 0; i < n; i++) {
        // Printing leading spaces
        for (int j = 0; j < i; j++) {
            printf(" ");
        }
        // Printing stars
        for (int k = 0; k < (2* (n-i)-1); k++) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}
