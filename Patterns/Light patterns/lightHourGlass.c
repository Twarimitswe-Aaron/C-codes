#include <stdio.h>

int main(){
	int size=5;
	for(int i=0;i<2*size+1;i++){
		int space=(i<size) ? (i): (2*size-i);
		int star=(i<size) ? 2* (size-i)+1: 2* (i-size)+1;
		for(int j=0;j<space;j++){
			printf(" ");
		}
		for(int k=0;k<star;k++){
			if(i==0 || i == 2*size|| k==0 || k== star-1){
				printf("*");
			}else{
				printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}