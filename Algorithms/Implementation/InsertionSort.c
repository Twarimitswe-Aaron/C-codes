#include <stdio.h>
void printArray(int *array,int size){
	printf("[");
	for(int i=0;i<size;i++){
		if(i>0){
			printf(",");
		}
		printf("%d",array[i]);
	}
	printf("]\n");
}
void insertionSort(int *array,int size){
	int key,j;
	for(int i=1;i<size;i++){
		key=array[i];
		j=i;
		while(j>0 && array[j-1]>key){
			array[j]=array[j-1];
			j--;
		}
		array[j]=key;
	}
	printf("The array after sorting is ");
	printArray(array,size);
}

int main(){
	int array[]={1,3,4,3,2,42,3};
	int size=sizeof(array)/sizeof(array[0]);
	printf("The array before sorting is ");
	printArray(array,size);
	insertionSort(array,size);
	return 0;
}