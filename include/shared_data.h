#ifndef __SHARED_DATA_H__
#define __SHARED_DATA_H__

/* Shared data library
 * ---------------------
 */

/* External libraries */
#include <semaphore.h>

/* Build-in libraries */
#include <stddef.h>

/* Data types */
struct shared_data_s {
    sem_t  sem1;
    sem_t  sem2;
    size_t size;
    char data[];
};

#endif  // __SHARED_DATA_H__

