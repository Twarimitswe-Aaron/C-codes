#include <stdio.h>

enum card{
	HEART=1,SPADE,DIAMOND,CLUB
};
int main(){
	enum card suit=HEART;
	printf("%d", suit);
	return 0;
}