#include <stdio.h>

int main(){
	int n=5;
	for(int i=0;i<n;i++){
		for(int j=0;j<n*2;j++){
			if(j==0 || j==2*n-1|| i==0 || i==n-1){
				printf("*");
			}else{
				printf(" ");
			}
			
		}
		printf("\n");
	}
	return 0;
} 