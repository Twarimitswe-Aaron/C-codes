#include <stdio.h>

enum bitflag{
	READ_ONLY=1,HIDDEN=2,ARCHIVE=4
};
int main(){
	enum bitflag bitflag_bit =READ_ONLY | HIDDEN ;
	if(bitflag_bit & HIDDEN){
		printf("File is hidden\n");
	}
	if(bitflag_bit & READ_ONLY){
		printf("File is read only\n");
	}
	if(bitflag_bit & ARCHIVE){
		printf("File is archive\n");
	}
}