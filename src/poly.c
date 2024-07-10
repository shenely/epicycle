#include <stdlib.h>
#include <assert.h>
#include "poly.h"
#include "log.h"

void poly_zero(struct poly_s* restrict P) {
    memset(&P->coeff, 0, (P->deg + 1) * sizeof(double));
}

void poly_pos(const struct poly_s* P, struct poly_s* restrict Q)
{
    LOG_STATS("poly_pos", 0, 0, 0);
    assert(Q->deg >= P->deg);
    Q->deg = P->deg;
    memcpy(&Q->coeff, &P->coeff, (P->deg + 1) * sizeof(double));
}

void poly_neg(const struct poly_s* P, struct poly_s* restrict Q)
{
    LOG_STATS("poly_neg", P->deg, 0, 0);
    assert(Q->deg >= P->deg);
    Q->deg = P->deg;
    for (size_t n = 0; n <= P->deg; n++)
        Q->coeff[n] = - P->coeff[n];
}

void poly_add(const struct poly_s* P, const struct poly_s* Q, struct poly_s* restrict R)
{
    LOG_STATS("poly_add", MAX(P->deg, Q->deg) - MIN(P->deg, Q->deg), 0, 0);
    assert(R->deg >= MAX(P->deg, Q->deg));
    R->deg = MAX(P->deg, Q->deg);
    for (size_t n = 0; n <= R->deg; n++)
        R->coeff[n] = ((n <= P->deg) ? P->coeff[n] : 0.0)
                    + ((n <= Q->deg) ? Q->coeff[n] : 0.0);
    while (R->coeff[R->deg] == 0.0) R->deg--;
}

void poly_sub(const struct poly_s* P, const struct poly_s* Q, struct poly_s* restrict R)
{
    LOG_STATS("poly_add", MAX(P->deg, Q->deg) - MIN(P->deg, Q->deg), 0, 0);
    assert(R->deg >= MAX(P->deg, Q->deg));
    R->deg = MAX(P->deg, Q->deg);
    for (size_t n = 0; n <= R->deg; n++)
        R->coeff[n] = ((n <= P->deg) ? P->coeff[n] : 0.0)
                    - ((n <= Q->deg) ? Q->coeff[n] : 0.0);
    while (R->coeff[R->deg] == 0.0) R->deg--;
}

void poly_mul(const struct poly_s* P, const struct poly_s* Q, struct poly_s* restrict R)
{
    LOG_STATS("poly_mul", P->deg * Q->deg, P->deg * Q->deg, 0);
    assert((P != R) && (Q != R));
    assert(R->deg >= P->deg + Q->deg);
    R->deg = P->deg + Q->deg;
    poly_zero(R);
    for (size_t n = 0; n <= P->deg; n++)
        for (size_t m = 0; m <= Q->deg; m++)
            R->coeff[n+m] += P->coeff[n] * Q->coeff[m];
    while (R->coeff[R->deg] == 0.0) R->deg--;
}

void poly_div(const struct poly_s* N, const struct poly_s* D, struct poly_s* restrict Q, struct poly_s* restrict R)
{
    LOG_STATS("poly_div", (N->deg - D->deg) * (D->deg + 2), (N->deg - D->deg) * (D->deg + 2), 0);
    assert((N != Q) && (D != Q) && (N != R) && (D != R));
    assert(R->deg >= N->deg);
    // N = D * Q + R
    poly_zero(Q);
    poly_pos(N, R);
    while (R->deg >= D->deg) {
        double t = R->coeff[R->deg] / D->coeff[D->deg];
        Q->coeff[R->deg - D->deg] += t;
        for (size_t n = 0; n <= D->deg; n++)
            R->coeff[n + R->deg - D->deg] -= t * D->coeff[n];
        while (R->coeff[R->deg] == 0.0) R->deg--;
    }
    while (Q->coeff[Q->deg] == 0.0) Q->deg--;
}

void poly_diff(const struct poly_s* P, struct poly_s* restrict diff_P) 
{
    LOG_STATS("poly_diff", 0, P->deg, 0);
    assert(P != diff_P);
    assert(diff_P->deg >= P->deg - 1);
    diff_P->deg = P->deg - 1;
    for (size_t n = 1; n <= P->deg; n++)
        diff_P->coeff[n-1] = n * P->coeff[n];
}

void poly_int(const struct poly_s* P, struct poly_s* restrict int_P) 
{
    LOG_STATS("poly_int", P->deg, P->deg, 0);
    assert(P != int_P);
    assert(int_P->deg >= P->deg + 1);
    int_P->deg = P->deg + 1;
    int_P->coeff[0] = 0.0;
    for (size_t n = 0; n <= P->deg; n++)
        int_P->coeff[n+1] = P->coeff[n] / (n + 1);
}

double poly_eval(const struct poly_s* P, double x) 
{
    LOG_STATS("poly_eval", P->deg, P->deg, 0);
    if (P->deg == 0) return 0.0;
    double y = P->coeff[P->deg];
    for (size_t n = P->deg - 1; n > 0; n--)
        y = y * x + P->coeff[n];
    return y * x + P->coeff[0];
}

