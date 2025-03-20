#include <stdio.h>

enum Color {
    Red, Green, Blue, Black, White
};

int main() {
    enum Color selectedColor = Blue;
    switch (selectedColor) {
        case Red:
            printf("Selected color: Red\n");
            break;
        case Green:
            printf("Selected color: Green\n");
            break;
        case Blue:
            printf("Selected color: Blue\n");
            break;
        case Black:
            printf("Selected color: Black\n");
            break;
        case White:
            printf("Selected color: White\n");
            break;
    }
    return 0;
}
