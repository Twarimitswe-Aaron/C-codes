#include <stdio.h>
#include <ctype.h>

int main(){
	char ch='A';
	printf("Capital letters\n");
	while(ch<='Z'){
		printf("%c, ",ch);
		ch++;
	}
	
	ch='A';
	printf("\nlower letters\n");
	while(ch<='Z'){
		printf("%c, ",tolower(ch));
		ch++;
	}
	return 0;
}