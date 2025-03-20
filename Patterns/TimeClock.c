#include <stdio.h>

int main(){
	int n=5;
	for(int i=0;i<n*2-1;i++){
		int spaces=(i<n)?(i):(2*n-i-2);
		int stars=2*(n-spaces)-1;
		for(int j=0;j<spaces;j++){
			printf(" ");
		}
		for(int k=0;k<stars;k++){
			printf("*");
		}
		printf("\n");
	}
	return 0;
}