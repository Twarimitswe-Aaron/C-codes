#include <stdio.h>

int main(){
	int n;
	printf("Multiplication tabel\n");
	printf("====================\n");
	printf("Enter the multiplicatio table you want:");
	scanf("%d", &n);
	for(int i=0;i<11;i++){
		printf("%d * %d = %d\n",n,i,n*i);
		
	}
}