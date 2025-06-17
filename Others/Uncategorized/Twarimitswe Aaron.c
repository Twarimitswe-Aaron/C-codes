#include <stdio.h>

// Function to compute the determinant of a 3x3 matrix
int determinant3x3(int m[3][3]) {
    int det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
            - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
            + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return det;
}

int main() {
    int size;

    printf("Enter the size of the matrix : ");
    scanf("%d", &size);

    int array[size][size];
    int numbers = size * size;

    printf("Enter %d numbers:\n", numbers);

    // Input elements
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &array[i][j]);
        }
    }

    // Pattern A: Diagonals
    printf("\nPattern A: Diagonals\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j || i + j == size - 1) {
                printf(" %d ", array[i][j]);
            } else {
                printf("    ");
            }
        }
        printf("\n");
    }

    // Pattern B: Upper Right Triangle
    printf("\nPattern B: Upper Right Triangle\n");
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            printf(" %d", array[i][j]);
        }
        printf("\n");
    }

    // Pattern C: Lower Right Triangle
    printf("\nPattern C: Lower Right Triangle\n");
    for (int i = 0; i < size; i++) {
        for (int j = size - i - 1; j < size; j++) {
            printf(" %d", array[i][j]);
        }
        printf("\n");
    }

    // Pattern D: Row sums
    printf("\nPattern D: All numbers with row sum\n");
    for (int i = 0; i < size; i++) {
        int sum = 0;
        for (int j = 0; j < size; j++) {
            printf(" %d", array[i][j]);
            sum += array[i][j];
        }
        printf(" | %d", sum);
        printf("\n");
    }

    // Pattern E: Column products
    printf("\nPattern E: All numbers with column product\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("  %d  ", array[i][j]);
        }
        printf("\n");
    }

    for (int j = 0; j < size; j++) {
        int product = 1;
        for (int i = 0; i < size; i++) {
            product *= array[i][j];
        }
        printf(" %d  ", product);
    }
    printf(" <-- Column products\n");

    // ? Pattern G: Determinant (only if 3x3)
    if (size == 3) {
        int det = determinant3x3(array);
        printf("\nPattern G: Determinant of 3x3 matrix = %d\n", det);
    } else {
        printf("\nPattern G: Determinant only supported for 3x3 matrices.\n");
    }

    return 0;
}
