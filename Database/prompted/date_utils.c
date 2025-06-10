#include <stdio.h>
#include <string.h>
#include "date_utils.h"

void formatDate(time_t timestamp, char* buffer) {
    if (timestamp == 0) {
        strcpy(buffer, "N/A");
        return;
    }
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, 20, "%Y-%m-%d", timeinfo);
}

time_t parseDate(const char* dateStr) {
    if (!dateStr || strlen(dateStr) == 0) return 0;
    
    struct tm timeinfo = {0};
    sscanf(dateStr, "%d-%d-%d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
    timeinfo.tm_year -= 1900;  // Years since 1900
    timeinfo.tm_mon -= 1;      // Months since January (0-11)
    
    return mktime(&timeinfo);
} 