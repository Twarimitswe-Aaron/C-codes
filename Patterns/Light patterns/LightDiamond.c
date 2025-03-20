#include <stdio.h>

int main() {
    int size = 5;

    // Loop over rows (size * 2 rows in total)
    for (int i = 0; i < size * 2; i++) {
        // Calculate spaces
        int space = (i < size) ? (size - i - 1) : (i - size + 1);
        
        // Calculate stars
        int stars = (i < size) ? (2 * i + 1) : (2 * (size - space) - 1);

        // Print spaces
        for (int j = 0; j < space; j++) {
            printf(" ");
        }

        // Print stars
        for (int k = 0; k < stars; k++) {
            if(i==2*size-1|| i==0 || k==0 || k== (i*2) || k== stars-1){
            	printf("*");
			}else{
				printf(" ");
			}
        }

        // Move to the next line after each row
        printf("\n");
    }

    return 0;
}
