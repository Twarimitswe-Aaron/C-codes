#include <stdio.h>

int main(){
	int size=15;
	for(int i=0;i<size;i++){
		for(int j=0;j<size-i;j++){
			printf(" ");
		}
		for(int k=0;k<2*i-1;k++){
			if(i==size-1 || k==0 || k==2*i-2){
				printf("*");
			}else{
				printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}