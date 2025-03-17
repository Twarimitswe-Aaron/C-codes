#include <stdio.h>

int main(){
	
	
	int size;
	printf("please enter the number of elements in array:");
	scanf("%d",&size);
	int array[size];
	int *pointer= array;
	for (int i=0;i<size;i++){
		if(i+1 == 1){
			printf("Enter the 1st number:");
			scanf("%d",&*(pointer+i));
			
		}
		else if(i+1 == 2){
			printf("Enter the 2nd number:");
			scanf("%d",&*(pointer+i));
		}
		else if(i+1 == 3){
		     printf("Enter the 3rd number:");
			scanf("%d",&*(pointer+i));
		}else{
			printf("Enter the %d th number:", i+1);
				scanf("%d", &*(pointer+i));
		}
	}
	printf("The values in array are \n");
	for(int j=0;j<size;j++){
		printf("%d at address of %p\n", *(pointer+j), &*(pointer+j));
	}
	return 0;
}