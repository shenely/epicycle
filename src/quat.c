#include <stddef.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "quat.h"
#include "util.h"
#include "log.h"

void quat_zero(quat_t* restrict q) {
    memset(q, 0, sizeof(quat_t));
}

void quat_one(quat_t* restrict q) 
{
    LOG_STATS("quat_one", 0, 0, 0);
    (*q)[0] = 1.0;
    vec_zero((vec_t*) &(*q)[1]);
}

void quat_pos(const quat_t* p, quat_t* restrict q) {
    memcpy(q, p, sizeof(quat_t));
}

void quat_neg(const quat_t* p, quat_t* restrict q)
{
    LOG_STATS("quat_neg", 5, 0, 0);
    for (size_t i = 0; i < 4; i++)
        (*q)[i] = - (*p)[i];
}

void quat_conj(const quat_t* p, quat_t* restrict q)
{
    LOG_STATS("quat_conj", 3, 0, 0);
    (*q)[0] = (*p)[0];
    for (size_t i = 1; i < 4; i++)
        (*q)[i] = - (*p)[i];
}

double quat_norm(const quat_t* q)
{
    LOG_STATS("quat_norm", 4, 4, 1);
    double s = 0.0;
    for (size_t i = 0; i < 4; i++)
        s += (*q)[i] * (*q)[i];
    return sqrt(s);
}

bool quat_inv(const quat_t* q, quat_t* restrict q_inv)
{
    LOG_STATS("quat_inv", 4, 5, 0);
    double s = 0.0;
    for (size_t i = 0; i < 4; i++)
        s += (*q)[i] * (*q)[i];
    if (s < M_ARCSEC)
        return false;
    quat_conj(q, q_inv);
    quat_muls(q_inv, 1.0 / s, q_inv);
    return true;
}

bool quat_unit(const quat_t* p, quat_t* restrict q)
{
    LOG_STATS("quat_unit", 0, 1, 0);
    double s;
    if ((s = quat_norm(p)) < M_ARCSEC)
        return false;
    quat_muls(p, 1.0 / s, q);
    return true;
}

bool quat_iszero(const quat_t* q) {
    return quat_norm(q) < M_ARCSEC;
}

bool quat_isunit(const quat_t* q) {
    return fabs(quat_norm(q) - 1.0) < M_ARCSEC;
}

void quat_add(const quat_t* p, const quat_t* q, quat_t* restrict r)
{
    LOG_STATS("quat_add", 4, 0, 0);
    for (size_t i = 0; i < 4; i++)
        (*r)[i] = (*p)[i] + (*q)[i];
}

void quat_sub(const quat_t* p, const quat_t* q, quat_t* restrict r)
{
    LOG_STATS("quat_sub", 4, 0, 0);
    for (size_t i = 0; i < 4; i++)
        (*r)[i] = (*p)[i] - (*q)[i];
}

void quat_muls(const quat_t* p, double s, quat_t* restrict q)
{
    LOG_STATS("quat_muls", 0, 4, 0);
    for (size_t i = 0; i < 4; i++)
        (*q)[i] = (*p)[i] * s;
}

void quat_mulv(const quat_t* p, const vec_t* v_bar, quat_t* restrict q)
{
    LOG_STATS("quat_mulv", 9, 12, 0);
    assert(p != q);
    (*q)[0] = - (*p)[1] * (*v_bar)[0]
              - (*p)[2] * (*v_bar)[1]
              - (*p)[3] * (*v_bar)[2];
    (*q)[1] = (*p)[0] * (*v_bar)[0]
            + (*p)[2] * (*v_bar)[2]
            - (*p)[3] * (*v_bar)[1];
    (*q)[2] = (*p)[0] * (*v_bar)[1]
            - (*p)[1] * (*v_bar)[2]
            + (*p)[3] * (*v_bar)[0];
    (*q)[3] = (*p)[0] * (*v_bar)[2]
            + (*p)[1] * (*v_bar)[1]
            - (*p)[2] * (*v_bar)[0];
}

void quat_vmul(const vec_t* v_bar, const quat_t* p, quat_t* restrict q)
{
    LOG_STATS("quat_vmul", 9, 12, 0);
    assert(p != q);
    (*q)[0] = - (*v_bar)[0] * (*p)[1]
              - (*v_bar)[1] * (*p)[2]
              - (*v_bar)[2] * (*p)[3];
    (*q)[1] = (*v_bar)[0] * (*p)[0]
            - (*v_bar)[2] * (*p)[2]
            + (*v_bar)[1] * (*p)[3];
    (*q)[2] = (*v_bar)[1] * (*p)[0]
            + (*v_bar)[2] * (*p)[1]
            - (*v_bar)[0] * (*p)[3];
    (*q)[3] = (*v_bar)[2] * (*p)[0]
            - (*v_bar)[1] * (*p)[1]
            + (*v_bar)[0] * (*p)[2];
}

void quat_mul(const quat_t* p, const quat_t* q, quat_t* restrict r)
{
    LOG_STATS("quat_mul", 12, 16, 0);
    (*r)[0] = (*p)[0] * (*q)[0]
            - (*p)[1] * (*q)[1]
            - (*p)[2] * (*q)[2]
            - (*p)[3] * (*q)[3];
    (*r)[1] = (*p)[0] * (*q)[1]
            + (*p)[1] * (*q)[0]
            + (*p)[2] * (*q)[3]
            - (*p)[3] * (*q)[2];
    (*r)[2] = (*p)[0] * (*q)[2]
            - (*p)[1] * (*q)[3]
            + (*p)[2] * (*q)[0]
            + (*p)[3] * (*q)[1];
    (*r)[3] = (*p)[0] * (*q)[3]
            + (*p)[1] * (*q)[2]
            - (*p)[2] * (*q)[1]
            + (*p)[3] * (*q)[0];
}

void quat_pow(const quat_t* q, double t, quat_t* restrict q__t)
{
    LOG_STATS("quat_pow", 1, 3, 3);
    assert(quat_isunit(q));
    double s = vec_dot((vec_t*) &(*q)[1], (vec_t*) &(*q)[1]);
    if (s < M_ARCSEC) {
        quat_one(q__t);
    } else {
        (*q__t)[0] = cos(t * acos((*q)[0]));
        s = sqrt((1.0 - (*q__t)[0] * (*q__t)[0]) / s);
        vec_muls((vec_t*) &(*q)[1], s, (vec_t*) &(*q__t)[1]);
    }
}

void vec_exp(const vec_t* v_bar, quat_t* restrict q)
{
    LOG_STATS("vec_exp", 1, 5, 2);
    double s = vec_norm(v_bar);
    if (s < M_ARCSEC) {
        quat_one(q);
    } else {
        (*q)[0] = cos(s);
        s = sqrt(1.0 - (*q)[0] * (*q)[0]) / s;
        vec_muls(v_bar, s, (vec_t*) &(*q)[1]);
    }
}

void quat_log(const quat_t* q, vec_t* restrict v_bar)
{
    LOG_STATS("quat_log", 0, 1, 1);
    assert(quat_isunit(q));
    double s = vec_norm((vec_t*) &(*q)[1]);
    if (s < M_ARCSEC) {
        vec_zero(v_bar);
    } else {
        s = acos((*q)[0]) / s;
        vec_muls((vec_t*) &(*q)[1], s, v_bar);
    }
}

void vec_rot(const quat_t* q, const vec_t* u_bar, vec_t* restrict v_bar)
{
    LOG_STATS("vec_rot", 0, 0, 0);
    assert(quat_isunit(q));
    quat_t foo, bar;
    quat_conj(q, &foo);
    quat_vmul(u_bar, &foo, &bar);
    quat_mul(q, &bar, &foo);
    assert(fabs(foo[0]) < M_ARCSEC);
    memcpy(v_bar, &(foo[1]), sizeof(vec_t));
}

void vec_irot(const quat_t* q, const vec_t* u_bar, vec_t* restrict v_bar)
{
    LOG_STATS("vec_irot", 0, 0, 0);
    assert(quat_isunit(q));
    quat_t foo, bar;
    quat_conj(q, &foo);
    quat_mulv(&foo, u_bar, &bar);
    quat_mul(&bar, q, &foo);
    assert(fabs(foo[0]) < M_ARCSEC);
    memcpy(v_bar, &(foo[1]), sizeof(vec_t));
}

void quat_rot_mat(const quat_t* q, mat_t* restrict R)
{
    LOG_STATS("quat_rot_mat", 12, 27, 0);
    assert(quat_isunit(q));
    for (size_t i = 0; i < 3; i++) {
        (*R)[i][i] = 1.0 - 2.0 * ((*q)[1+(i+2)%3] * (*q)[1+(i+2)%3] + (*q)[1+(i+1)%3] * (*q)[1+(i+1)%3]);
        (*R)[(i+2)%3][(i+1)%3] = 2.0 * ((*q)[1+(i+2)%3] * (*q)[1+(i+1)%3] + (*q)[1+i] * (*q)[0]);
        (*R)[(i+1)%3][(i+2)%3] = 2.0 * ((*q)[1+(i+1)%3] * (*q)[1+(i+2)%3] - (*q)[1+i] * (*q)[0]);
    }
}

void quat_irot_mat(const quat_t* q, mat_t* restrict R)
{
    LOG_STATS("quat_irot_mat", 12, 27, 0);
    assert(quat_isunit(q));
    for (size_t i = 0; i < 3; i++) {
        (*R)[i][i] = 1.0 - 2.0 * ((*q)[1+(i+2)%3] * (*q)[1+(i+2)%3] + (*q)[1+(i+1)%3] * (*q)[1+(i+1)%3]);
        (*R)[(i+2)%3][(i+1)%3] = 2.0 * ((*q)[1+(i+2)%3] * (*q)[1+(i+1)%3] - (*q)[1+i] * (*q)[0]);
        (*R)[(i+1)%3][(i+2)%3] = 2.0 * ((*q)[1+(i+1)%3] * (*q)[1+(i+2)%3] + (*q)[1+i] * (*q)[0]);
    }
}

