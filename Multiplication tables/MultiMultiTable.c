#include <stdio.h>

int main(){
	printf("MULTIPLICATION TABLE\n====================\n");
	int maxSize=10;
	for(int i=1;i<=maxSize;i++){
		printf("Multiplication table %d \n", i);
		for(int j=0;j<maxSize;j++){
			printf("%d * %d = %d \n", i,j,i*j);
		}
	}
	return 0;
}