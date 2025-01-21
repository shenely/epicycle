#ifndef __QUAT_H__
#define __QUAT_H__

/* Quaternion library
 * ------------------
 */
/* Internal libraries */
#include "mat.h"
#include "util.h"
#include "vec.h"

/* Built-in libraries */
#include <math.h>
#include <stdbool.h>
#include <string.h>

/* Data types */
typedef double quat_t[4];

/* Zero quaternion
 * :param quat_t q: output quaternion
 */
static inline
void
quat_zero(
    quat_t q
) {
    memset(q, 0, sizeof(quat_t));
}

/* Unitary quaternion
 * :param quat_t q: output quaternion
 */
void quat_one(quat_t);

/* Quaternion positive
 * :param quat_t p: input quaternion
 * :param quat_t q: output quaternion
 */
static inline
void
quat_pos(
    const quat_t p,
    quat_t q
) {
    memcpy(q, p, sizeof(quat_t));
}

/* Quaternion negative
 * :param quat_t p: input quaternion
 * :param quat_t q: output quaternion
 */
void quat_neg(const quat_t, quat_t);

/* Quaternion conjugate
 * :param quat_t p: input quaternion
 * :param quat_t q: output quaternion
 */
void quat_conj(const quat_t, quat_t);

/* Quaternion norm
 * :param quat_t q: input quaternion
 * :return double: output scalar
 */
double quat_norm(const quat_t);

/* Quaternion inverse
 * :param quat_t q: input quaternion
 * :param quat_t q_inv: output quaternion
 * :returns bool: invertable quaternion
 */
bool quat_inv(const quat_t, quat_t);

/* Unit quaternion
 * :param quat_t p: input quaternion
 * :param quat_t q: output quaternion
 * :returns bool: invertable quaternion
 */
bool quat_unit(const quat_t, quat_t);

/* Is zero quaternion?
 * :param quat_t q: input quaternion
 * :returns: is zero quaternion?
 * :rtype: bool
 */
static inline
bool
quat_iszero(
    const quat_t q
) {
    return quat_norm(q) < M_ARCSEC;
}

/* Is unit quaternion?
 * :param quat_t q: input quaternion
 * :returns: is unit quaternion?
 * :rtype: bool
 */
static inline
bool
quat_isunit(
    const quat_t q
) {
    return fabs(quat_norm(q) - 1.0) < M_ARCSEC;
}

/* Quaternion addition
 * :param quat_t p: (first) input quaternion
 * :param quat_t q: (second) input quaternion
 * :param quat_t r: output quaternion
 */
void quat_add(const quat_t, const quat_t, quat_t);

/* Quaternion subtraction
 * :param quat_t p: (first) input quaternion
 * :param quat_t q: (second) input quaternion
 * :param quat_t r: output quaternion
 */
void quat_sub(const quat_t, const quat_t, quat_t);

/* Quaternion-scalar multiplication
 * :param quat_t p: input quaternion
 * :param double s: input scalar
 * :param quat_t q: output quaternion
 */
void quat_muls(const quat_t, double, quat_t);

/* Quaternion-vector multiplication
 * :param quat_t p: input quaternion
 * :param vec_t* v_bar: input vector
 * :param quat_t q: output quaternion
 */
void quat_mulv(const quat_t, const vec_t, quat_t);

/* Vector-muaternion multiplication
 * :param vec_t* v_bar: input vector
 * :param quat_t p: input quaternion
 * :param quat_t q: output quaternion
 */
void quat_vmul(const vec_t, const quat_t, quat_t);

/* Quaternion multiplication
 * :param quat_t p: (first) input quaternion
 * :param quat_t q: (second) input quaternion
 * :param quat_t r: output quaternion
 */
void quat_mul(const quat_t, const quat_t, quat_t);

/* Quaternion exponentiation
 * :param quat_t p: input quaternion
 * :param double t: input scalar
 * :param quat_t q: output quaternion
 */
void quat_pow(const quat_t, double, quat_t);

/* Vector exponentiation
 * :param vec_t* v_bar: input vector
 * :param quat_t q: output (unit) quaternion
 */
void vec_exp(const vec_t, quat_t);

/* Quaternion logarithm
 * :param quat_t q: input (unit) quaternion
 * :param vec_t* v_bar: output vector
 */
void quat_log(const quat_t, vec_t);

/* Vector rotation
 * :param quat_t q: input (unit) quaternion
 * :param vec_t* u_bar: input vector
 * :param vec_t* v_bar: output vector
 */
void vec_rot(const quat_t, const vec_t, vec_t);

/* Vector (inverse) rotation
 * :param quat_t q: input (unit) quaternion
 * :param vec_t* u_bar: input vector
 * :param vec_t* v_bar: output vector
 */
void vec_irot(const quat_t, const vec_t, vec_t);

/* Quaternion rotation matrix
 * :param quat_t q: input (unit) quaternion
 * :param mat_t* R: output matrix
 */
void quat_rot_mat(const quat_t, mat_t);

/* Quaternion (inverse) rotation matrix
 * :param quat_t q: input (unit) quaternion
 * :param mat_t* R: output matrix
 */
void quat_irot_mat(const quat_t, mat_t);

#endif  // __QUAT_H__

