#include <stdio.h>

int main(){
	int number=10;
	int variable=20;
	int *firstPointer= &number;
	int **secondPointer= &firstPointer;
	
	 printf("the value of this element is %d\n", number);
	 printf("The 1st pointer with %p address has  %d\n\n", (void *)firstPointer, *firstPointer);
	 printf("The 2nd pointer with %p address references the value of first pointer %d\n", (void *)secondPointer, **secondPointer);
	 
	 //Modifying the variable
	 **secondPointer= variable;
	 
	 printf("the value of this element is %d\n\n", number);
	 printf("The 1st pointer with %p address has changed to  %d\n\n", (void *)firstPointer, *firstPointer);
	 printf("The 2nd pointer with %p address references the value of first pointer %d", (void *)secondPointer, **secondPointer);
}





