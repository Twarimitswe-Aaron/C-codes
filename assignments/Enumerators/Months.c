#include <stdio.h>


enum Month {
    JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE, 
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
};


void printDays(enum Month month) {
	const char *monthNames[] = {
    "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE",
    "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"
};
int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month >= JANUARY && month <= DECEMBER) {
        printf("%s has %d days.\n",monthNames[month], daysInMonth[month]);
    } else {
        printf("Invalid month selection.\n");
    }
}

int main() {
    enum Month month = NOVEMBER;
    printDays(month);
    return 0;
}
