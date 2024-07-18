#include <stddef.h>
#include "mat.h"
#include "config.h"
#include "util.h"
#include "log.h"

void mat_eye(mat_t A) 
{
    LOG_STATS("mat_eye", 0, 0, 0);
    memset(A, 0, sizeof(mat_t));
    for (size_t i = 0; i < 3; i++)
        A[i][i] = 1.0;
}

void mat_neg(const mat_t A, mat_t B)
{
    LOG_STATS("mat_neg", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        vec_neg(A[i], B[i]);
}

double mat_tr(const mat_t A)
{
    LOG_STATS("mat_tr", 3, 0, 0);
    double s = 0.0;
    for (size_t i = 0; i < 3; i++)
        s += A[i][i];
    return s;
}

double mat_det(const mat_t A)
{
    LOG_STATS("mat_det", 6, 9, 0);
    double s = 0.0;
    for (size_t i = 0; i < 3; i++)
        s += A[0][i] * (
            + A[1][(i+1)%3] * A[2][(i+2)%3]
            - A[1][(i+2)%3] * A[2][(i+1)%3]
        );
    return s;
}

bool mat_inv(const mat_t A, mat_t A_inv)
{
    LOG_STATS("mat_inv", 18, 27, 0);
    double s;
    if (fabs(s = mat_det(A)) < ABSTOL)
        return false;
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            A_inv[j][i] = (
                + A[(i+1)%3][(j+1)%3] * A[(i+2)%3][(j+2)%3]
                - A[(i+1)%3][(j+2)%3] * A[(i+2)%3][(j+1)%3]
            ) / s;
    return true;
}

void mat__T(const mat_t A, mat_t A__T)
{
    LOG_STATS("mat__T", 0, 0, 0);
    double temp;
    for (size_t i = 0; i < 3; i++) {
        A__T[i][i] = A[i][i];
        for (size_t j = i + 1; j < 3; j++) {
            temp = A[i][j];
            A__T[i][j] = A[j][i];
            A__T[j][i] = temp;
        }
    }
}

void mat_add(const mat_t A, const mat_t B, mat_t C)
{
    LOG_STATS("mat_add", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        vec_add(A[i], B[i], C[i]);
}

void mat_sub(const mat_t A, const mat_t B, mat_t C)
{
    LOG_STATS("mat_sub", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        vec_sub(A[i], B[i], C[i]);
}

void mat_muls(const mat_t A, double s, mat_t B)
{
    LOG_STATS("mat_muls", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        vec_muls(A[i], s, B[i]);
}

void mat_mul(const mat_t A, const mat_t B, mat_t C)
{
    LOG_STATS("mat_mul", 27, 27, 0);
    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++) {
            C[i][j] = 0.0;
            for (size_t k = 0; k < 3; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
}

/* Matrix/vector functions */

void vec_omul(const vec_t u_bar, const vec_t v_bar, mat_t A)
{
    LOG_STATS("vec_omul", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        vec_muls(v_bar, u_bar[i], A[i]);
}

void mat_mulv(const mat_t A, const vec_t u_bar, vec_t v_bar)
{
    LOG_STATS("mat_mulv", 0, 0, 0);
    for (size_t i = 0; i < 3; i++)
        v_bar[i] = vec_dot(A[i], u_bar);
}

void mat_vmul(const vec_t u_bar, const mat_t A, vec_t v_bar)
{
    LOG_STATS("mat_vmul", 9, 9, 0);
    for (size_t i = 0; i < 3; i++) {
        v_bar[i] = 0.0;
        for (size_t j = 0; j < 3; j++)
            v_bar[i] += u_bar[j] * A[j][i];
    }
}

void vec_cross_mat(const vec_t v_bar, mat_t A)
{
    LOG_STATS("vec_cross_mat", 3, 3, 0);
    A[0][0] = A[1][1] = A[2][2] = 0.0;
    for (size_t i = 0; i < 3; i++) {
        A[(i+2)%3][(i+1)%3] = + v_bar[i];
        A[(i+1)%3][(i+2)%3] = - v_bar[i];
    }
}

