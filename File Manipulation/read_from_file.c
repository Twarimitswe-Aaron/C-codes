#include <stdio.h>
#include <conio.h>

int main(){
	FILE *fp;
	char ch;
	fp=fopen("My_sample.txt", "r");
	printf("Reading character from the file %c", getc(fp));
	ch=getc(fp);
	printf("ch = %c",ch);
	fclose(fp);
	getch();
	return 0;
}