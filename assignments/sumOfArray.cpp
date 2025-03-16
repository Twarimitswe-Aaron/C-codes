#include <stdio.h>

void sumOfArray(int *array,int size){

	int sum=0;
	
	for(int i=0;i<size;i++){
		sum+=array[i];
	}
	printf("%d",sum);
}
int main(){
	
	int array[]={1,2,3};
	int size=sizeof(array)/sizeof(array[0]);
	sumOfArray(array,size);
	
	return 0;
}