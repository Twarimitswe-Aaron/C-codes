#include <stdio.h>
#include <stdlib.h>

void copyTheArray(int *array,int size){
	int *tempArray=(int *)malloc(size * sizeof(int));
	for(int i=0;i<size;i++){
		tempArray[i]=array[i];
	}
	for(int i=0;i<size;i++){
		printf("%d, ",tempArray[i]);
	}
	free(tempArray);

}
int main(){
	int array[]={1,2,3,4,5,6,7};
	int size=sizeof(array)/sizeof(array[0]);
	copyTheArray(array, size);
	
	
	
	return 0;
}