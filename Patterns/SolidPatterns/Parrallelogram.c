#include <stdio.h>

int main(){
	int n=5;
	for(int i=n;i>0;i--){
		for(int j=i;j>0;j--){
			printf(" ");
		}
		for(int k=0;k<n*2;k++){
			printf("*");
		}
		printf("\n");
	}
	return 0;
}