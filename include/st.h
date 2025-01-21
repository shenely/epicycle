#ifndef __ST_H__
#define __ST_H__

/* State library
 * -------------
 */
 
/* Internal libraries */
#include "quat.h"
#include "vec.h"

/* Built-in libraries */
#include <string.h>
 
/* Data types */
typedef
struct {
    vec_t r_bar;  // position vector
    quat_t q;  // rotation quaternion
    vec_t v_bar;  // velocity vector
    vec_t om_bar;  // angular velocity vector
} st_t;

/* Zero state
 * :param st_t* a: output state
 */
void st_zero(st_t* restrict);

/* State positive
 * :param st_t* a: input state
 * :param st_t* b: output state
 */
static inline
void
st_pos(
    const st_t* a,
    st_t* restrict b
) {
    memcpy(b, a, sizeof(st_t));
}

/* State negative
 * :param st_t* a: input state
 * :param st_t* b: output state
 */
void st_neg(const st_t*, st_t*);

/* State addition
 * :param st_t* a: (first) input state
 * :param st_t* b: (second) input state
 * :param st_t* c: output state
 */
void st_add(const st_t*, const st_t*, st_t*);

/* State subtraction
 * :param st_t* a: (first) input state
 * :param st_t* b: (second) input state
 * :param st_t* c: output state
 */
void st_sub(const st_t*, const st_t*, st_t*);

/* State-scalar multiplication
 * :param st_t* a: input state
 * :param double s: input scalar
 * :param st_t* a: output state
 */
void st_muls(const st_t*, double, st_t*);

/* State interplation
 * :param double x0:
 * :param st_t* y0: (first) input state
 * :param double x1:
 * :param st_t* y1: (second) input state
 * :param double x:
 * :param st_t* y: output state
 */
void st_interp(double, const st_t*, double, const st_t*, double, st_t* restrict);

/* State integration
 * :param double delta_x:
 * :param st_t* k: state derivative
 * :param st_t* y0: input state
 * :param st_t* y1: output state
 */
void st_int(double, const st_t*, const st_t*, st_t* restrict);

#endif  // __ST_H__

