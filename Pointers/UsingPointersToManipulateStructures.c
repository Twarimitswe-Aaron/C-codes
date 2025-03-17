#include <stdio.h>

struct car {
    const char *name;
    const char *color;
    int age;
};

int main() {
    struct car car1 = {"Benz", "White", 12};

  
    struct car *pointer = &car1;

  
    printf("Name: %s\n", pointer->name);
    printf("Color: %s\n", pointer->color);
    printf("Age: %d\n", pointer->age);

 
    pointer->name = "Awudi";  
    pointer->color = "Black";  
    pointer->age = 14;         

 
    printf("Name: %s\n", pointer->name);
    printf("Color: %s\n", pointer->color);
    printf("Age: %d\n", pointer->age);

    return 0;
}
