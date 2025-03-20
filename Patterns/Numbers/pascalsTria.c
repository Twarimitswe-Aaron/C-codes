#include <stdio.h>
#include <stdlib.h>

#define BLACK "\033[30m"
#define WHITE "\033[47m"
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

int main() {
    int row, isValid = 0;
    
    printf("PASCALS' TRIANGLE\n==================\n");

    while (!isValid) {
        printf("\nEnter the size of Pascal's Triangle you want(1-20): ");
        if (scanf("%d", &row) != 1 || row <= 0) {
            printf("%sPlease enter a valid positive integer\n___________________________%s\n", RED, RESET);
            while (getchar() != '\n'); // Clear invalid input
        }else if(row>20){
        	printf("\n%sCan't handle rows above 20 please use others below %s'", RED,RESET);
        	while (getchar() != '\n');
		}
		 else {
            isValid = 1;
        }
    }

    // Allocate memory for Pascal's Triangle
    int **array = malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++) {
        array[i] = malloc((i + 1) * sizeof(int));
    }

    // Generate Pascal's Triangle
    for (int i = 0; i < row; i++) {
        // Print spaces for pyramid alignment
        for (int k = 0; k < row - i; k++) {
            printf("  ");
        }

        // Print Pascal's Triangle numbers
        printf("%s ", GREEN);
        for (int j = 0; j <= i; j++) {
            if (j == 0 || j == i) {
                array[i][j] = 1;
            } else {
                array[i][j] = array[i-1][j-1] + array[i-1][j];
            }
            printf("%5d", array[i][j]);  // Ensures consistent spacing
        }

        printf("\n"); // Move to next row
    }
    printf("%s", RESET);

    // Free allocated memory
    for (int i = 0; i < row; i++) {
        free(array[i]);
    }
    free(array);

    return 0;
}
