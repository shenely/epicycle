#ifndef __LOG_H__
#define __LOG_H__
/* Logging utilities
 * --------------------
 * Helper functions for logging messages
 */

#include <time.h>

/* Defined logging levels */
enum log_e {
    E_TRACE    =  0,
    E_DEBUG    = 10,
    E_INFO     = 20,
    E_WARNING  = 30,
    E_ERROR    = 40,
    E_CRITICAL = 50
};
#define LOG_LEVEL_UP 10

#define LOG(level, color, ...) log_any(level, #level, color, __VA_ARGS__)
#ifdef __DEBUG__
#define LOG_TRACE(...)    LOG(E_TRACE, "01;34", __VA_ARGS__)
#define LOG_DEBUG(...)    LOG(E_DEBUG, "01;32", __VA_ARGS__)
extern clock_t stats_tick, stats_tock;
extern long stats_total_add,
     stats_total_mul,
     stats_total_pow;
#define START_CLOCK() stats_tick = clock()
#define STOP_CLOCK() stats_tock = clock()
#define LOG_STATS(name, add, mul, pow) (\
    stats_total_add += add,\
    stats_total_mul += mul,\
    stats_total_pow += pow,\
    LOG_TRACE("%s: +:%d, *:%d, ^:%d", name, add, mul, pow)\
)
#define RESET_STATS() (\
    stats_tick = 0,\
    stats_tock = 0,\
    stats_total_add = 0,\
    stats_total_mul = 0,\
    stats_total_pow = 0\
)
#define SHOW_STATS() LOG_DEBUG(\
    "t:%.0fus, +:%d, *:%d, ^:%d", \
    1e6 * (stats_tock - stats_tick) / CLOCKS_PER_SEC,\
    stats_total_add, stats_total_mul, stats_total_pow\
)
#else
#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_STATS(name, add, mul, pow)
#define RESET_STATS()
#define SHOW_STATS()
#endif
#define LOG_INFO(...)     LOG(E_INFO, "01;37", __VA_ARGS__)
#define LOG_WARNING(...)  LOG(E_WARNING, "01;33", __VA_ARGS__)
#define LOG_ERROR(...)    LOG(E_ERROR, "01;05;31", __VA_ARGS__)
#define LOG_CRITICAL(...) LOG(E_CRITICAL, "01;05;35", __VA_ARGS__)

/* Generate logging message
 * :param log_e level: logging level
 * :param char* text: entry prefix
 * :param char* color: terminal color
 * :param char* format: message format
 */
void log_any(enum log_e, const char*, const char*, const char*, ...);

#endif  // __LOG_H__

