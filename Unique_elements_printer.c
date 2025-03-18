#include <stdio.h>
#include <stdlib.h>

void checkUnique(int size) {
    int *array = (int *)malloc(size * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter the elements you want in the array:\n");
    for (int i = 0; i < size; i++) {
    	printf("index [%d]:",i);
        scanf("%d", &array[i]);
    }

    // Bubble sort
    int swapped;
    for (int i = 0; i < size - 1; i++) {
        swapped = 0;
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }

    // Printing unique elements
    printf("Unique elements: [");
    int first = 1; // Flag to handle commas
    for (int k = 0; k < size; k++) {
        if (k == 0 || array[k] != array[k - 1]) {
            if (!first) {
                printf(", ");
            }
            printf("%d", array[k]);
            first = 0;
        }
    }
    printf("]\n");

    free(array);
}

int main() {
    int size;
    printf("Unique elements printer\n=======================\n");
    printf("Enter the size of the array: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid array size!\n");
        return 1;
    }

    checkUnique(size);
    return 0;
}
