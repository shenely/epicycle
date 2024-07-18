#ifndef __GMAT_H__
#define __GMAT_H__
/* General vector/matrix library
 * -----------------------------
 */
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "util.h"

#if !defined GMAT_NDIM
#error "General matrix dimension undefined"
#endif

#define gvec_imul(u_bar, v_bar) gvec_dot(u_bar, v_bar)

/* Data types */
typedef double gvec_t[GMAT_NDIM]; // 13-vector
typedef double gmat_t[GMAT_NDIM][GMAT_NDIM]; // 13x13 matrix

/* General zero vector
 * :param gvec_t v_bar: output vector
 */
static inline void gvec_zero(gvec_t v_bar) {
    memset(v_bar, 0, sizeof(gvec_t));
}

/* General vector positive
 * :param gvec_t u_bar: input vector
 * :param gvec_t v_bar: output vector
 */
static inline void gvec_pos(const gvec_t u_bar, gvec_t v_bar) {
    memcpy(v_bar, u_bar, sizeof(gvec_t));
}

/* General vector functions */

/* General vector negative
 * :param gvec_t u_bar: input vector
 * :param gvec_t v_bar: output vector
 */
void gvec_neg(const gvec_t, gvec_t);

/* General vector norm
 * :param gvec_t u_bar: input vector
 * :return double: output scalar
 */
double gvec_norm(const gvec_t);

/* General unit vector
 * :param gvec_t v_bar: input vector
 * :param gvec_t v_hat: output (unit) vector
 */
bool gvec_unit(const gvec_t, gvec_t);

/* Is general zero vector?
 * :param vec_t gvec_t: input vector
 * :returns: is zero vector?
 * :rtype: bool
 */
static inline bool gvec_iszero(const gvec_t v_bar) {
    return gvec_norm(v_bar) < ABSTOL;
}

/* Is general unit vector?
 * :param gvec_t v_bar: input vector
 * :returns: is unit vector?
 * :rtype: bool
 */
static inline bool gvec_isunit(const gvec_t v_bar) {
    return fabs(gvec_norm(v_bar) - 1.0) < ABSTOL;
}

/* General vector addition
 * :param gvec_t u_bar: (first) input vector
 * :param gvec_t v_hat: (second) input vector
 * :param gvec_t w_hat: output vector
 */
void gvec_add(const gvec_t, const gvec_t, gvec_t);

/* General vector subtraction
 * :param gvec_t u_bar: (first) input vector
 * :param gvec_t v_hat: (second) input vector
 * :param gvec_t w_hat: output vector
 */
void gvec_sub(const gvec_t, const gvec_t, gvec_t);

/* General vector-scalar multiplication
 * :param gvec_t u_bar: input vector
 * :param double s: input scalar
 * :param gvec_t v_hat: output vector
 */
void gvec_muls(const gvec_t, double, gvec_t);

/* General ector dot product
 * :param gvec_t u_bar: (first) input vector
 * :param gvec_t v_hat: (second) input vector
 * :returns double: output scalar
 */
double gvec_dot(const gvec_t, const gvec_t);

/* General matrix functions */

/* General zero matrix
 * :param mat_t* A: output matrix
 */
static inline void gmat_zero(gmat_t A) {
    memset(A, 0, sizeof(gmat_t));
}

/* General identity matrix
 * :param gmat_t I: output matrix
 */
void gmat_eye(gmat_t);

/* General matrix positive
 * :param gmat_t A: input matrix
 * :param gmat_t B: output matrix
 */
static inline void gmat_pos(const gmat_t A, gmat_t B) {
    memcpy(B, A, sizeof(gmat_t));
}

/* General matrix negative
 * :param gmat_t A: input matrix
 * :param gmat_t B: output matrix
 */
void gmat_neg(const gmat_t, gmat_t);

/* General matrix trace
 * :param gmat_t A: input matrix
 * :returns double: output scalar
 */
double gmat_tr(const gmat_t);

/* General matrix transpose
 * :param gmat_t A: input matrix
 * :param gmat_t A__T: output matrix
 */
void gmat__T(const gmat_t, gmat_t);

/* Generla matrix addition
 * :param gmat_t A: (first) input matrix
 * :param gmat_t B: (second) input matrix
 * :param gmat_t C: output matrix
 */
void gmat_add(const gmat_t, const gmat_t, gmat_t);

/* Generla matrix subtraction
 * :param gmat_t A: (first) input matrix
 * :param gmat_t B: (second) input matrix
 * :param gmat_t C: output matrix
 */
void gmat_sub(const gmat_t, const gmat_t, gmat_t);

/* Generla matrix-scalar multiplication
 * :param gmat_t A: input matrix
 * :param double s: input scalar
 * :param gmat_t B: output matrix
 */
void gmat_muls(const gmat_t, double, gmat_t);

/* General matrix multiplication
 * :param gmat_t A: (first) input matrix
 * :param gmat_t B: (second) input matrix
 * :param gmat_t C: output matrix
 */
void gmat_mul(const gmat_t, const gmat_t, gmat_t);

/* General matrix/vector functions */

/* Generla vector outer product
 * :param gvec_t u_bar: (first) input vector
 * :param gvec_t v_bar: (second) input vector
 * :param gmat_t A: output matrix
 */
void gvec_omul(const gvec_t, const gvec_t, gmat_t);

/* General matrix-vector multiplication
 * :param gmat_t A: input matrix
 * :param gvec_t u_bar: input vector
 * :param gvec_t v_bar: output vector
 */
void gmat_mulv(const gmat_t, const gvec_t, gvec_t);

/* General vector-matrix multiplication
 * :param gvec_t u_bar: input vector
 * :param gmat_t A: input matrix
 * :param gvec_t v_bar: output vector
 */
void gmat_vmul(const gvec_t, const gmat_t, gvec_t);

#endif  // __GMAT_H__

