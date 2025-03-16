#include <stdio.h>
#include <stdlib.h>

// Function to create an array
int* createArray(int *size) {
    int inputValid = 0;

    while (!inputValid) {
        printf("Simple array creator and printer\n");
        printf("================================\n");
        printf("Enter the size of the array: ");

        if (scanf("%d", size) == 1) {
            if (*size > 0) {
                inputValid = 1; // Input is valid, exit the loop
            } else {
                printf("\nSize must be a positive number. Please try again.\n\n");
            }
        } else {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a valid number.\n\n");
        }
    }

    // Allocate memory for the array
    int *array = (int *)malloc(*size * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        exit(1); // Exit the program if memory allocation fails
    }

    // Fill the array with user input
    printf("Enter the value of the array\n");
    for (int i = 0; i < *size; i++) {
        printf("index[%d]: ", i);
        while (scanf("%d", &array[i]) != 1) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a valid integer.\n");
            printf("index[%d]: ", i);
        }
    }

    return array; // Return the created array
}

// Function to print the array
void printArray(int *array, int size) {
    printf("\nPrint array\n=========\n");
    for (int i = 0; i < size; i++) {
        printf("index[%d]: %d\n", i, array[i]);
    }
}

int main() {
    int size;
    int *array = createArray(&size); // Create the array

    printArray(array, size); // Print the array

    // Free the allocated memory
    free(array);

    return 0;
}