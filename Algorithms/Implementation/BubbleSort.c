#include <stdio.h>

//swapp elements
void swapp(int *array,int current,int next){
	int temp=array[current];
	array[current]=array[next];
	array[next]=temp;
	
}
void printArray(int *array,int size){
	printf("[");
	for(int i=0;i<size;i++){
		if(i>0 &&  i<size){
			printf(",");
		}
		printf("%d",array[i]);
	}
	printf("]\n");
}
void bubbleSort(int *array,int size){
	int swapped;
	for (int i=0;i<size;i++){
		swapped=0;
		for(int j=0;j<size-i-1;j++){
			if(array[j]>array[j+1]){
			swapp(array,j,j+1);
			swapped=1;
			}
		}
		if(!swapped){
		  break;
		}
	}

	printf("The array after sorting is ");
	printArray(array,size);
}
int main(){
	int array[]={6,4,3,2,1,234,56,65,3466,908,1000};
	int size=sizeof(array)/sizeof(array[0]);
	printf("The array before sorting is ");
	printArray(array,size);
	bubbleSort(array,size);
	return 0;
}