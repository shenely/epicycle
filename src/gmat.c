#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "gmat.h"
#include "config.h"
#include "log.h"

/* General vector functions */
void gvec_zero(gvec_t* restrict v_bar) {
    memset(v_bar, 0, sizeof(gvec_t));
}

void gvec_pos(const gvec_t* u_bar, gvec_t* restrict v_bar) {
    memcpy(v_bar, u_bar, sizeof(gvec_t));
}

void gvec_neg(const gvec_t* u_bar, gvec_t* restrict v_bar)
{
    LOG_STATS("gvec_neg", GMAT_NDIM, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        (*v_bar)[i] = - (*u_bar)[i];
}

double gvec_norm(const gvec_t* v_bar)
{
    LOG_STATS("gvec_norm", GMAT_NDIM, GMAT_NDIM, 1);
    double s = 0.0;
    for (size_t i = 0; i < GMAT_NDIM; i++)
        s += (*v_bar)[i] * (*v_bar)[i];
    return sqrt(s);
}

bool gvec_unit(const gvec_t* v_bar, gvec_t* restrict v_hat)
{
    LOG_STATS("gvec_unit", 0, 1, 0);
    double s;
    if ((s = gvec_norm(v_bar)) < ABSTOL)
        return false;
    gvec_muls(v_bar, 1.0 / s, v_hat);
    return true;
}

bool gvec_iszero(const gvec_t* v_bar) {
    return gvec_norm(v_bar) < ABSTOL;
}

bool gvec_isunit(const gvec_t* v_bar) {
    return fabs(gvec_norm(v_bar) - 1.0) < ABSTOL;
}

void gvec_add(const gvec_t* u_bar, const gvec_t* v_bar, gvec_t* restrict w_bar)
{
    LOG_STATS("gvec_add", GMAT_NDIM, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        (*w_bar)[i] = (*u_bar)[i] + (*v_bar)[i];
}

void gvec_sub(const gvec_t* u_bar, const gvec_t* v_bar, gvec_t* restrict w_bar)
{
    LOG_STATS("gvec_sub", GMAT_NDIM, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        (*w_bar)[i] = (*u_bar)[i] - (*v_bar)[i];
}

void gvec_muls(const gvec_t* u_bar, double s, gvec_t* restrict v_bar)
{
    LOG_STATS("gvec_muls", 0, GMAT_NDIM, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        (*v_bar)[i] = (*u_bar)[i] * s;
}

double gvec_dot(const gvec_t* u_bar, const gvec_t* v_bar)
{
    LOG_STATS("gvec_dot", GMAT_NDIM, GMAT_NDIM, 0);
    double s = 0.0;
    for (size_t i = 0; i < GMAT_NDIM; i++)
        s += (*u_bar)[i] * (*v_bar)[i];
    return s;
}

/* General matrix/vector functions */
void gmat_zero(gmat_t* restrict A) {
    memset(A, 0, sizeof(gmat_t));
}

void gmat_eye(gmat_t* restrict A) 
{
    LOG_STATS("gmat_eye", 0, 0, 0);
    memset(A, 0, sizeof(gmat_t));
    for (size_t i = 0; i < GMAT_NDIM; i++)
        (*A)[i][i] = 1.0;
}

void gmat_pos(const gmat_t* A, gmat_t* B) {
    memcpy(B, A, sizeof(gmat_t));
}

void gmat_neg(const gmat_t* A, gmat_t* B)
{
    LOG_STATS("gmat_neg", 0, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        gvec_neg(&(*A)[i], &(*B)[i]);
}

double gmat_tr(const gmat_t* A)
{
    LOG_STATS("gmat_tr", GMAT_NDIM, 0, 0);
    double s = 0.0;
    for (size_t i = 0; i < GMAT_NDIM; i++)
        s += (*A)[i][i];
    return s;
}

void gmat__T(const gmat_t* A, gmat_t* A__T)
{
    LOG_STATS("gmat__T", 0, 0, 0);
    double temp;
    for (size_t i = 0; i < GMAT_NDIM; i++) {
        (*A__T)[i][i] = (*A)[i][i];
        for (size_t j = i + 1; j < GMAT_NDIM; j++) {
            temp = (*A)[i][j];
            (*A__T)[i][j] = (*A)[j][i];
            (*A__T)[j][i] = temp;
        }
    }
}

bool gmat_inv(const gmat_t* A, gmat_t* restrict A_inv);

void gmat_add(const gmat_t* A, const gmat_t* B, gmat_t* C)
{
    LOG_STATS("gmat_add", 0, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        gvec_add(&(*A)[i], &(*B)[i], &(*C)[i]);
}

void gmat_sub(const gmat_t* A, const gmat_t* B, gmat_t* C)
{
    LOG_STATS("gmat_add", 0, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        gvec_sub(&(*A)[i], &(*B)[i], &(*C)[i]);
}


void gmat_muls(const gmat_t* A, double s, gmat_t* B)
{
    LOG_STATS("gmat_muls", 0, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        gvec_muls(&(*A)[i], s, &(*B)[i]);
}

void gmat_mul(const gmat_t* A, const gmat_t* B, gmat_t* restrict C)
{
    LOG_STATS("gmat_mul", GMAT_NDIM * GMAT_NDIM * GMAT_NDIM, GMAT_NDIM * GMAT_NDIM * GMAT_NDIM, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        for (size_t j = 0; j < GMAT_NDIM; j++) {
            (*C)[i][j] = 0.0;
            for (size_t k = 0; k < GMAT_NDIM; k++)
                (*C)[i][j] += (*A)[i][k] * (*B)[k][j];
        }
}

/* General matrix/vector functions */

void gvec_omul(const gvec_t* u_bar, const gvec_t* v_bar, gmat_t* restrict A)
{
    LOG_STATS("gvec_omul", 0, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        gvec_muls(v_bar, (*u_bar)[i], &(*A)[i]);
}

void gmat_mulv(const gmat_t* A, const gvec_t* u_bar, gvec_t* restrict v_bar)
{
    LOG_STATS("gmat_mulv", 0, 0, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++)
        (*v_bar)[i] = gvec_dot(&(*A)[i], u_bar);
}

void gmat_vmul(const gvec_t* u_bar, const gmat_t* A, gvec_t* restrict v_bar)
{
    LOG_STATS("gmat_mulv", GMAT_NDIM * GMAT_NDIM, GMAT_NDIM * GMAT_NDIM, 0);
    for (size_t i = 0; i < GMAT_NDIM; i++) {
        (*v_bar)[i] = 0.0;
        for (size_t j = 0; j < GMAT_NDIM; j++)
            (*v_bar)[i] += (*u_bar)[j] * (*A)[j][i];
    }
}

