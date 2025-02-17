#ifndef __DMAT_H__
#define __DMAT_H__

/* Diagonal matrix library
 * -----------------------
 */
 
/* Internal libraries */
#include "mat.h"
#include "vec.h"

/* Built-in libraries */
#include <stdbool.h>

/* Data type */
typedef vec_t dmat_t; // 3x3 (diagonal) matrix

#define dmat_zero(A) vec_zero(A, B)

/* Diagonal identity matrix
 * :param dmat_t I: output matrix
 */
void dmat_eye(dmat_t);

/* Diagonal matrix trace
 * :param dmat_t A: input matrix
 * :returns double: output scalar
 */
double dmat_tr(const dmat_t);

/* Diagonal matrix determinant
 * :param dmat_t A: input matrix
 * :returns double: output scalar
 */
double dmat_det(const dmat_t);

/* Diagonal matrix inverse
 * :param dmat_t A: input matrix
 * :param dmat_t A_inv: output matrix
 * :returns bool: matrix invertable
 */
bool dmat_inv(const dmat_t, dmat_t);

#define dmat_pos(A, B) vec_pos(A, B)
#define dmat_neg(A, B) vec_neg(A, B)

#define dmat_muls(A, s, C) vec_muls(A, s, C)

/* Diagonal matrix multiplication
 * :param dmat_t A: (first) input matrix
 * :param dmat_t B: (second) input matrix
 * :param dmat_t C: output matrix
 */
void dmat_mul(const dmat_t, const dmat_t, dmat_t);

/* Matrix-(diagonal) matrix multiplication
 * :param mat_t A: (first) input matrix
 * :param dmat_t B: (second) input matrix
 * :param mat_t C: output matrix
 */
void dmat_muld(const mat_t, const dmat_t, mat_t);

/* (Diagonal) matrix-matrix multiplication
 * :param dmat_t A: (first) input matrix
 * :param mat_t B: (second) input matrix
 * :param mat_t C: output matrix
 */
void dmat_dmul(const dmat_t, const mat_t, mat_t);

#define dmat_mulv(A, u_bar, v_bar) dmat_mul(A, u_bar, v_bar)
#define dmat_vmul(u_bar, A, v_bar) dmat_mul(A, u_bar, v_bar)

#endif  // __DMAT_H__

