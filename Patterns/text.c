#include <stdio.h>

int main(){
	int size=6;

	int array[6][6]={{1,2,3,4,5,6},{1,2,3,4,6,7},{1,2,3,4,7,6},{1,2,3,4,5,4},{1,2,3,4,6,5},{1,2,3,4,6,5}};
	for(int i=0;i<=size;i++){
		for(int j=0;j<=i;j++){
			printf(" ");
		}
		for(int k=5;k>=i;k--){
			printf("%d", array[i][k]);
		}
		printf("\n");
	}
	
	return 0;
}