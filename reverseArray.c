#include <stdio.h>
#include <stdlib.h>

void reverse(int *array, int size) {
    int *reversedArray = (int *)malloc(size * sizeof(int));
    if (reversedArray == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    int j = 0;
    for (int i = size - 1; i >= 0; i--) {
        reversedArray[j] = array[i];
        j++;
    }

    // Copy back to the original array
    for (int i = 0; i < size; i++) {
        array[i] = reversedArray[i];
    }

    free(reversedArray); // Free allocated memory
}

void printArray(int *array, int size){
	
	for (int i=0;i<size;i++){
		printf("%d ,",array[i]);
	}
	
}

int main(){
	int array[]={1,2,3,4};
	int size=sizeof(array)/sizeof(array[0]);
	printf("The array before is :");
	printArray(array,size);
	printf("\nThe reversed array is:");
	reverse(array,size);
	printArray(array,size);
	
	
	return 0;
}