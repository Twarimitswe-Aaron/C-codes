#include <stdio.h>

int main(){
	int rows=4;
	int no=1;
	for(int i=0;i<rows;i++){
		for(int j=0;j<=i;j++){
			printf("  %d  ",no++);
		}
		printf("\n\n");
	}
	return 0;
}