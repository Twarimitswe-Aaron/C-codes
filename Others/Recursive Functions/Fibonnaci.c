#include <stdio.h>

// Correct ANSI escape codes for red text and reset
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

int fibonacci(int n) {
    if (n <= 0) return 0;         
    if (n == 1) return 1;         
    return fibonacci(n - 1) + fibonacci(n - 2); // Recursive case
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int pos, isValid = 0;

    while (!isValid) {
        printf("FIBONACCI SEQUENCE\n");
        printf("==================\n");
        printf("Enter the position of fibonacci: ");  // Fix the printf for red text
        
        if (scanf("%d", &pos) != 1 || pos < 0) {
            printf("%s\nPlease enter a valid position number (non-negative integer)%s\n", RED, RESET);  // Fix the invalid input message
            clearBuffer();
        } else {
            printf("%sFibonacci number at position %d is %d%s\n", GREEN, pos, fibonacci(pos), RESET);  // Fixed placement of RESET
            isValid = 1;
        }
    }

    return 0;
}
