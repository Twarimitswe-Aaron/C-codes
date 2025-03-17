#include <stdio.h>

int main() {
    int arr[] = {10, 20, 30, 40, 50};
    int key;
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Enter the number you want to search:");
    scanf("%d", &key);
    int *ptr = arr, found = 0;

    for (int i = 0; i < n; i++) {
        if (*(ptr + i) == key) {
            printf("Element %d found at index %d\n", key, i);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Element %d not found\n", key);
    }

    return 0;
}
