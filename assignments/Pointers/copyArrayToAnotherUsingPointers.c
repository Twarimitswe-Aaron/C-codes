#include <stdio.h>

int main() {
    int size1 = 3;
    int array1[10] = {1, 2, 3}; 
    int array2[] = {4, 5, 6, 7, 8, 9, 10};

    int size2 = sizeof(array2) / sizeof(array2[0]); 

    int *pointerToArray1 = array1 + size1; 
    int *pointerToArray2 = array2;

    
    for (int i = 0; i < size2; i++) {
        *pointerToArray1 = *pointerToArray2; 
        pointerToArray1++; 
        pointerToArray2++; 
    }

   
    int totalSize = size1 + size2;

  
    printf("Values which were and are copied are: ");
    for (int j = 0; j < totalSize; j++) {
        printf("%d, " ,array1[j]);
    }
    printf("\n");

    return 0;
}
