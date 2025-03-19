#include <stdio.h>

float sum(float n){
	if(n==1){
		return 1;
	}else{
	   return 1/n + sum(n-1);
	}
}
int main(){
	int valid=0;
	float n;
    while(!valid){
    	printf("\nSPECIFIC CALCULATOR\n");
    	printf("====================\n");
    	printf("This is working about 1+1/2+1/3 ... +1/n\n");
	printf("Enter n to find the sum:");
	if(scanf("%f",&n) !=1){
		printf("Please enter a valid number");
	}else{
		valid=1;
	}
	}
	printf("The sum is %g",sum(n));
	return 0;
}