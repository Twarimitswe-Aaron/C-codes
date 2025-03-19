#include <stdio.h>
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
void selectionSort(int *array,int size){
	int minIndex;
	for (int i=0;i<size;i++){
		minIndex=i;
		for(int j=i+1;j<size;j++){
			if(array[j]<array[minIndex]){
				minIndex=j;
			}
		}
		if(minIndex!=i){
			swapp(array,minIndex,i);
		}
	}
	printf("The array after sorting is ");
	printArray(array,size);
}
int main(){
	int array[]={6,5,41,1,3,2,1};
	int size=sizeof(array)/sizeof(array[0]);
	printf("The array before sorting is ");
	printArray(array,size);
	selectionSort(array,size);
	return 0;
}