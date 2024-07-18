#include <stddef.h>
#include "dmat.h"
#include "config.h"
#include "log.h"

void dmat_eye(dmat_t A) 
{
    LOG_STATS("dmat_eye", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        A[i] = 1.0;
}

double dmat_tr(const dmat_t A)
{
    LOG_STATS("dmat_tr", 3, 0, 0);
    double value = 0.0;
    for (size_t i = 0; i < 3; i++)
        value += A[i];
    return value;
}

double dmat_det(const dmat_t A)
{
    LOG_STATS("dmat_det", 0, 3, 0);
    double value = 1.0;
    for (size_t i = 0; i < 3; i++)
        value *= A[i];
    return value;
}

bool dmat_inv(const dmat_t A, dmat_t A_inv)
{
    LOG_STATS("dmat_det", 0, 1, 0);
    double s;
    if (fabs(s = dmat_det(A)) < ABSTOL)
        return false;
    dmat_muls(A, 1.0 / s, A_inv);
    return true;
}

void dmat_mul(const dmat_t A, const dmat_t B, dmat_t C)
{
    LOG_STATS("dmat_mul", 0, 3, 0);
    for (size_t i = 0; i < 3; i++)
        C[i] = A[i] * B[i];
}

void dmat_muld(const mat_t A, const dmat_t B, mat_t C)
{
    LOG_STATS("dmat_muld", 0, 9, 0);
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            C[i][j] = A[i][j] * B[j];
}

void dmat_dmul(const dmat_t A, const mat_t B, mat_t C)
{
    LOG_STATS("dmat_muld", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        vec_muls(B[i], A[i], C[i]);
}

