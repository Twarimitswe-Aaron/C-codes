#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <time.h>

void formatDate(time_t timestamp, char* buffer);
time_t parseDate(const char* dateStr);

#endif // DATE_UTILS_H 