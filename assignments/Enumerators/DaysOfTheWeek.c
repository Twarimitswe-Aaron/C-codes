#include <stdio.h>

enum dayOfTheWeek{sunday,monday,tuesday,wednesday,thursday,friday,saturday};
int main(){
	enum dayOfTheWeek day=wednesday;
	printf("%d", day);
	return 0;
}