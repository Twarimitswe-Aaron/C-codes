#include<stdio.h>
 int main(){
	int number=1;
	char name[7]="Aart";
	float decimal=2.454;
	
	//pointers assigned to those variables
	
	int *pointerNumber=&number;
	char *pointerName= name;
	float *pointerDecimal=&decimal;
	
	//printing memory address and their values
	
	printf("The value is %d and the memory address is %p\n", *pointerNumber, (void *)pointerNumber);
	printf("The value is %s and the memory address is %p\n", pointerName, (void *)pointerName);
	printf("The value is %f and the memory address is %p\n", *pointerDecimal, (void *)pointerDecimal);
	return 0;
	}