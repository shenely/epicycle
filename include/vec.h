#ifndef __VEC_H__
#define __VEC_H__
/* Vector library
 * --------------
 */
#include <stdbool.h>
#include "config.h"

/* Data types */
typedef double vec_t[3]; // 3-vector

/* Constants */
extern const vec_t i_hat, j_hat, k_hat;  // cardinal unit vectors

/* Zero vector
 * :param vec_t* v_bar: output vector
 */
void vec_zero(vec_t* restrict);

/* Vector positive
 * :param vec_t* u_bar: input vector
 * :param vec_t* v_bar: output vector
 */
void vec_pos(const vec_t*, vec_t*);

/* Vector negative
 * :param vec_t* u_bar: input vector
 * :param vec_t* v_bar: output vector
 */
void vec_neg(const vec_t*, vec_t*);

/* Vector norm
 * :param vec_t* u_bar: input vector
 * :returns: output scalar
 * :rtype: double
 */
double vec_norm(const vec_t*);

/* Unit vector
 * :param vec_t* v_bar: input vector
 * :param vec_t* v_hat: output (unit) vector
 */
bool vec_unit(const vec_t*, vec_t*);

/* Is zero vector?
 * :param vec_t v_bar: input vector
 * :returns: is zero vector?
 * :rtype: bool
 */
bool vec_iszero(const vec_t*);

/* Is unit vector?
 * :param vec_t v_bar: input vector
 * :returns: is unit vector?
 * :rtype: bool
 */
bool vec_isunit(const vec_t*);

/* Vector addition
 * :param vec_t* u_bar: (first) input vector
 * :param vec_t* v_hat: (second) input vector
 * :param vec_t* w_hat: output vector
 */
void vec_add(const vec_t*, const vec_t*, vec_t*);

/* Vector subtraction
 * :param vec_t* u_bar: (first) input vector
 * :param vec_t* v_hat: (second) input vector
 * :param vec_t* w_hat: output vector
 */
void vec_sub(const vec_t*, const vec_t*, vec_t*);

/* Vector-scalar multiplication
 * :param vec_t* u_bar: input vector
 * :param double s: input scalar
 * :param vec_t* v_hat: output vector
 */
void vec_muls(const vec_t*, double, vec_t*);

/* Vector dot product
 * :param vec_t* u_bar: (first) input vector
 * :param vec_t* v_hat: (second) input vector
 * :returns: output scalar
 * :rtype: double
 */
double vec_dot(const vec_t*, const vec_t*);

/* Vector cross product
 * :param vec_t* u_bar: (first) input vector
 * :param vec_t* v_hat: (second) input vector
 * :param vec_t* w_hat: output vector
 */
void vec_cross(const vec_t*, const vec_t*, vec_t* restrict);

#endif  // __VEC_H__

