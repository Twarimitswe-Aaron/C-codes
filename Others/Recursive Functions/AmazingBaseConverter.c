#include <stdio.h>
#include <stdlib.h>  
#include <limits.h> 


#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"  


void clearInputBuffer() {
    while (getchar() != '\n');  
}


void baseConverter(long long int number, int base) {
    char conversion[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (number < 0) {
        printf(RED "-");  
        number = -number;
    }

    if (number >= base) {  
        baseConverter(number / base, base);
    }
    printf(YELLOW "%c" RESET, conversion[number % base]); 
}

int main() {
    long long int number;
    int base;
    int validInput = 0; 

    printf(MAGENTA "\nBASE CONVERTER\n" RESET);
    printf(BLUE "==============\n" RESET);

    
    while (!validInput) {
        printf(CYAN "Enter the number to convert in decimal: " RESET);
        if (scanf("%lld", &number) != 1) {  
            printf(RED "Error: Please enter a valid decimal number!\n" RESET);
            clearInputBuffer();
        } else {
            validInput = 1;  
        }
    }

    validInput = 0; 

 
    while (!validInput) {
        printf(CYAN "Enter the base you want to convert to (2-34): " RESET);
        if (scanf("%d", &base) != 1) {
            printf(RED "Error: Please enter a valid base!\n" RESET);
            clearInputBuffer();
        } else if (base < 2 || base > 34) {
            printf(RED "Error: Base must be between 2 and 34!\n" RESET);
        } else {
            validInput = 1;  
        }
    }

  
    printf(GREEN "The number " YELLOW "%lld" GREEN " in base " YELLOW "%d" GREEN " is " RESET, number, base);
    if (number == 0) {
        printf(YELLOW "0\n" RESET);
    } else {
        baseConverter(number, base);
        printf("\n");
    }

    return 0;
}
