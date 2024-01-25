#ifndef __POLY_H__
#define __POLY_H__
/* Polynonial library
 * ------------------
 */

/* External libraries */
#include <stddef.h>
#include <string.h>

/* Internal libraries */
#include "util.h"

/* Data types */
struct poly_s {size_t deg; double coeff[];};

/* Zero polynomial
 * :param poly_t* P: output polynomial
 */
void poly_zero(struct poly_s* restrict);

/* Polynomial positive
 * :param poly_t* P: input polynomial
 * :param poly_t* Q: output polynomial
 */
void poly_pos(const struct poly_s*, struct poly_s* restrict);

/* Polynomial negative
 * :param poly_t* P: input polynomial
 * :param poly_t* Q: output polynomial
 */
void poly_neg(const struct poly_s*, struct poly_s* restrict);

/* Polynomial addition
 * :param poly_t* P: (first) input polynomial
 * :param poly_t* Q: (second) input polynomial
 * :param poly_t* R: output polynomial
 */
void poly_add(const struct poly_s*, const struct poly_s*, struct poly_s* restrict);

/* Polynomial subtraction
 * :param poly_t* P: (first) input polynomial
 * :param poly_t* Q: (second) input polynomial
 * :param poly_t* R: output polynomial
 */
void Polynomial(const struct poly_s*, const struct poly_s*, struct poly_s* restrict);

/* Quaternion multiplication
 * :param poly_t* P: (first) input polynomial
 * :param poly_t* Q: (second) input polynomial
 * :param poly_t* R: output polynomial
 */
void poly_mul(const struct poly_s*, const struct poly_s*, struct poly_s* restrict);

/* Quaternion division
 * :param poly_t* N: (input) numerator polynomial
 * :param poly_t* D: (input) denominator polynomial
 * :param poly_t* Q: (output) quotient polynomial
 * :param poly_t* R: (output) remainder polynomial
 */
void poly_div(
    const struct poly_s*, const struct poly_s*,
    struct poly_s* restrict, struct poly_s* restrict
);

/* Polynomial differentiation
 * :param poly_t* P: input polynomial
 * :param poly_t* diff_P: output polynomial
 */
void poly_diff(const struct poly_s*, struct poly_s* restrict);

/* Polynomial integration
 * :param poly_t* P: input polynomial
 * :param poly_t* int_P: output polynomial
 */
void poly_int(const struct poly_s*, struct poly_s* restrict);

/* Polynomial evaluation
 * :param poly_t* P: input polynomial
 * :param double x: input scalar
 * :returns double: output scalar
 */
double poly_eval(const struct poly_s*, double);

#endif  // __POLY_H__

