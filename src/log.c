#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"

extern enum log_e noise_level;

#ifdef __DEBUG__
clock_t stats_tick = 0,
        stats_tock = 0;
long stats_total_add = 0,
     stats_total_mul = 0,
     stats_total_pow = 0;
#endif

void log_any(
    enum log_e level,
    const char* text,
    const char* color,
    const char* format, ...
) {
    if (level < noise_level) return;
    
    char timestamp[20];
    time_t now = time(NULL);
    struct tm* timeinfo = gmtime(&now);
    strftime(timestamp, 20, "%FT%TZ", timeinfo);
    
    char buffer[256];
    va_list vargs;
    va_start(vargs, format);
    vsprintf(buffer, format, vargs);
    va_end(vargs);
    
    printf("\x1B[01;36m(%s) \x1B[%sm[%s] \x1B[00m%s\n", timestamp, color, text, buffer);
}

