#include <stdio.h>

int main(){
	
	int n=5;
	for(int i=n;i>=0;i--){
		for (int j=0;j<=i;j++){
			printf("*");
		}
		printf("\n");
	}
	return 0;
}
