#include <stddef.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "vec.h"
#include "log.h"

const vec_t i_hat = {1.0, 0.0, 0.0},
            j_hat = {0.0, 1.0, 0.0},
            k_hat = {0.0, 0.0, 1.0};

void vec_zero(vec_t* restrict v_bar) {
    memset(v_bar, 0, sizeof(vec_t));
}

void vec_pos(const vec_t* u_bar, vec_t* restrict v_bar) {
    memcpy(v_bar, u_bar, sizeof(vec_t));
}

void vec_neg(const vec_t* u_bar, vec_t* restrict v_bar)
{
    LOG_STATS("vec_neg", 3, 0, 0);
    for (size_t i = 0; i < 3; i++)
        (*v_bar)[i] = - (*u_bar)[i];
}

double vec_norm(const vec_t* v_bar)
{
    LOG_STATS("vec_norm", 3, 3, 1);
    double s = 0.0;
    for (size_t i = 0; i < 3; i++)
        s += (*v_bar)[i] * (*v_bar)[i];
    return sqrt(s);
}

bool vec_unit(const vec_t* v_bar, vec_t* restrict v_hat)
{
    LOG_STATS("vec_unit", 0, 1, 0);
    double s;
    if ((s = vec_norm(v_bar)) < ABSTOL)
        return false;
    vec_muls(v_bar, 1.0 / s, v_hat);
    return true;
}

bool vec_iszero(vec_t* restrict v_bar) {
    return vec_norm(v_bar) < ABSTOL;
}

bool vec_isunit(vec_t* restrict v_bar) {
    return fabs(vec_norm(v_bar) - 1.0) < ABSTOL;
}

void vec_add(const vec_t* u_bar, const vec_t* v_bar, vec_t* restrict w_bar)
{
    LOG_STATS("vec_add", 3, 0, 0);
    for (size_t i = 0; i < 3; i++)
        (*w_bar)[i] = (*u_bar)[i] + (*v_bar)[i];
}

void vec_sub(const vec_t* u_bar, const vec_t* v_bar, vec_t* restrict w_bar)
{
    LOG_STATS("vec_sub", 3, 0, 0);
    for (size_t i = 0; i < 3; i++)
        (*w_bar)[i] = (*u_bar)[i] - (*v_bar)[i];
}

void vec_muls(const vec_t* u_bar, double s, vec_t* restrict v_bar)
{
    LOG_STATS("vec_muls", 0, 3, 0);
    for (size_t i = 0; i < 3; i++)
        (*v_bar)[i] = (*u_bar)[i] * s;
}

double vec_dot(const vec_t* u_bar, const vec_t* restrict v_bar)
{
    LOG_STATS("vec_dot", 3, 3, 0);
    double s = 0.0;
    for (size_t i = 0; i < 3; i++)
        s += (*u_bar)[i] * (*v_bar)[i];
    return s;
}

void vec_cross(const vec_t* u_bar, const vec_t* v_bar, vec_t* restrict w_bar)
{
    LOG_STATS("vec_cross", 3, 6, 0);
    assert((u_bar != w_bar) && (v_bar != w_bar));
    for (size_t i = 0; i < 3; i++)
        (*w_bar)[i] = (*u_bar)[(i+1)%3] * (*v_bar)[(i+2)%3]
                    - (*u_bar)[(i+2)%3] * (*v_bar)[(i+1)%3];
}

