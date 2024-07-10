#ifndef __UTIL_H__
#define __UTIL_H__
/* Utiliity functions
 * ----------------
 */

#include <stdint.h>

#define MIN(A, B) ((A<B)?A:B)
#define MAX(A, B) ((A>B)?A:B)

#define M_PI 3.14159265358979323846264338327
#define M_PI_180 (M_PI / 180)
#define M_180_PI (180 / M_PI)
#define M_DEGREE M_PI_180 / 2
#define M_ARCMIN M_DEGREE / 60
#define M_ARCSEC M_ARCMIN / 60

/* Swap pointers
 * :param void* A: first pointer
 * :param void* B: second pointer
 */
inline void SWAP(void* A, void* B)
{
    intptr_t temp = *((intptr_t*) A);
    *((intptr_t*) A) = *((intptr_t*) B);
    *((intptr_t*) B) = temp;
}

#endif  // __UTIL_H__

