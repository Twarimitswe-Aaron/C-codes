#include <stdio.h>

int main(){
	int size=5;
	for(int i=0;i<size;i++){
		for(int j=0;j<i;j++){
			printf(" ");
		}
		for(int k=0;k<2*(size-i)-1;k++){
			if(k==0 || k==2*(size-i)-2 || i==0){
				printf("*");
			}else{
				printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}