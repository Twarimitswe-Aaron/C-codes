#include <stdio.h>

enum ErrorCode {
    ERROR_NONE, ERROR_FILE_NOT_FOUND, ERROR_ACCESS_DENIED, ERROR_UNKNOWN
};

void handleError(enum ErrorCode error) {
    switch (error) {
        case ERROR_NONE:
            printf("No errors.\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("File not found.\n");
            break;
        case ERROR_ACCESS_DENIED:
            printf("Access denied.\n");
            break;
        case ERROR_UNKNOWN:
            printf("Unknown error.\n");
            break;
    }
}

int main() {
    enum ErrorCode error = ERROR_FILE_NOT_FOUND;
    handleError(error);
    return 0;
}
