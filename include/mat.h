#ifndef __MAT_H__
#define __MAT_H__
/* Matrix library
 * --------------
 */
#include <stdbool.h>
#include "vec.h"

#define vec_imul(u_bar, v_bar) vec_dot(u_bar, v_bar)

/* Data types */
typedef double mat_t[3][3]; // 3x3 matrix
typedef const double *const *const cmat_t; // 3x3 matrix

/* Zero matrix
 * :param mat_t A: output matrix
 */
static inline void mat_zero(mat_t A) {
    memset(A, 0, sizeof(mat_t));
}

/* Identity matrix
 * :param mat_t I: output matrix
 */
void mat_eye(mat_t);

/* Matrix positive
 * :param mat_t A: input matrix
 * :param mat_t B: output matrix
 */
static inline void mat_pos(const mat_t A, mat_t B) {
    memcpy(B, A, sizeof(mat_t));
}

/* Matrix negative
 * :param mat_t A: input matrix
 * :param mat_t B: output matrix
 */
void mat_neg(const mat_t, mat_t);

/* Matrix trace
 * :param mat_t A: input matrix
 * :returns double: output scalar
 */
double mat_tr(const mat_t);

/* Matrix determinant
 * :param mat_t A: input matrix
 * :returns double: output scalar
 */
double mat_det(const mat_t);

/* Matrix inverse
 * :param mat_t A: input matrix
 * :param mat_t A_inv: output matrix
 * :returns bool: invertable matrix
 */
bool mat_inv(const mat_t, mat_t);

/* Matrix transpose
 * :param mat_t A: input matrix
 * :param mat_t A__T: output matrix
 */
void mat__T(const mat_t, mat_t);

/* Matrix addition
 * :param mat_t A: (first) input matrix
 * :param mat_t B: (second) input matrix
 * :param mat_t C: output matrix
 */
void mat_add(const mat_t, const mat_t, mat_t);

/* Matrix subtraction
 * :param mat_t A: (first) input matrix
 * :param mat_t B: (second) input matrix
 * :param mat_t C: output matrix
 */
void mat_sub(const mat_t, const mat_t, mat_t);

/* Matrix-scalar multiplication
 * :param mat_t A: input matrix
 * :param double s: input scalar
 * :param mat_t B: output matrix
 */
void mat_muls(const mat_t, double, mat_t);

/* Matrix multiplication
 * :param mat_t A: (first) input matrix
 * :param mat_t B: (second) input matrix
 * :param mat_t C: output matrix
 */
void mat_mul(const mat_t, const mat_t, mat_t);

/* Matrix/vector functions */

/* Vector outer product
 * :param vec_t u_bar: (first) input vector
 * :param vec_t v_bar: (second) input vector
 * :param mat_t A: output matrix
 */
void vec_omul(const vec_t, const vec_t, mat_t);

/* Matrix-vector multiplication
 * :param mat_t A: input matrix
 * :param vec_t u_bar: input vector
 * :param vec_t v_bar: output vector
 */
void mat_mulv(const mat_t, const vec_t, vec_t);

/* Vector-matrix multiplication
 * :param vec_t u_bar: input vector
 * :param mat_t A: input matrix
 * :param vec_t v_bar: output vector
 */
void mat_vmul(const vec_t, const mat_t, vec_t);

/* Cross product matrix
 * :param vec_t u_bar: input vector
 * :param mat_t A: output matrix
 */
void vec_cross_mat(const vec_t, mat_t);

#endif  // __MAT_H__

