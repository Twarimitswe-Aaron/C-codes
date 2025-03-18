#include <stdio.h>
#include <stdlib.h>

void findDuplicate(int *array, int size) {
    int swapped, temp;
    
    // Bubble Sort
    for (int i = 0; i < size - 1; i++) {
        swapped = 0;
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }

    // Allocate memory for duplicate storage
    int *dupArray = (int *)malloc(size * sizeof(int));
    if (dupArray == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    int k = 0;
    int foundDuplicate = 0;
    int noDuplic=0;

    // Finding duplicates
    for (int i = 0; i < size - 1; i++) {  // Avoid accessing array[i+1] out of bounds
        if (array[i] == array[i + 1]) {
            if (k == 0 || dupArray[k - 1] != array[i]) {  // Avoid duplicate entries in dupArray
                dupArray[k++] = array[i];
            }
            foundDuplicate = 1;
            noDuplic++;
        }
    }

    // Print results
    if (!foundDuplicate) {
        printf("There are no duplicates in this array.\n");
    } else {
        printf("The duplicates are: ");
        for (int i = 0; i < k; i++) {
            printf("%d ", dupArray[i]);
            
        }
        
        printf("\n");
    }
    printf("%d number of duplicates", noDuplic);

    // Free allocated memory
    free(dupArray);
}

int main() {
    int array[] = {7, 6, 5, 4, 3,1,2,1,3,4, 2, 1, 1};
    int size = sizeof(array) / sizeof(array[0]);
    findDuplicate(array, size);
    return 0;
}
