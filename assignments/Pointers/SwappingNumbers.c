#include <stdio.h>

int main(){
	int number1=1;
	int number2=2;
	
	printf("The first value is %d\n", number1);
	printf("The second value is %d\n", number2);
	
	int num1=number1;
	int num2=number2;
	
	int *pointer1=&num1;
	int *pointer2=&num2;
	
	 number1=*pointer2;
	 number2=*pointer1;
	
	printf("The first value is changed to %d\n", number1);
	printf("The second value is changed to %d\n", number2);
	
	return 0;
}