#include <assert.h>
#include <math.h>
#include "ode.h"
#include "interp.h"
#include "quat.h"
#include "vec.h"
#include "config.h"
#include "util.h"
#include "log.h"

ode_meth_t ode_default_meth = ODE_METHOD_NAME(midp);

static void __interp_gvec(
    double x0, const gvec_t y0,
    double x1, const gvec_t y1,
    double x, gvec_t y
) {
    LOG_STATS("__interp_gvec", 0, 0, 0);
    interp_vlerp(
        x0, &y0[0],
        x1, &y1[0],
        x, &y[0]
    );
    interp_slerp(
        x0, &y0[3],
        x1, &y1[3],
        x, &y[3]
    );
    interp_vlerp(
        x0, &y0[7],
        x1, &y1[7],
        x, &y[7]
    );
    interp_vlerp(
        x0, &y0[10],
        x1, &y1[10],
        x, &y[10]
    );
}

static void __int_gvec(
    double delta_x,
    const gvec_t k,
    const gvec_t y0,
    gvec_t y1
) {
    LOG_STATS("__int_gvec", 0, 0, 0);
    vec_t foo;
    quat_t bar;
    gvec_t delta_y;
    gvec_muls(k, delta_x, delta_y);
    // r1_bar = r0_bar + delta_x * v_bar
    vec_add(&y0[0], &delta_y[0], &y1[0]);
    // q1 = q0 * exp(delta_x * om_bar / 2)
    vec_muls(&delta_y[4], 0.5, foo);
    vec_exp(foo, &delta_y[3]);
    quat_mul(&y0[3], &delta_y[3], bar);
    quat_pos(bar, &y1[3]);
    // v1_bar = v0_bar + delta_x * v_dot
    vec_add(&y0[7], &delta_y[7], &y1[7]);
    // om1_bar = om0_bar + delta_x * om_dot
    vec_add(&y0[10], &delta_y[10], &y1[10]);
}

static void __apply_stage(
    size_t N, const double* A,
    double delta_x,
    const gvec_t y0,
    gvec_t y1,
    const gvec_t k[]
) {
    LOG_STATS("__apply_stage", 0, N, 0);
    __int_gvec(A[0] * delta_x, k[0], y0, y1);
    for (size_t i = 1; i < N; i++) {
        if (fabs(A[i]) < ABSTOL)
            continue;
        gvec_t temp;
        __int_gvec(A[i] * delta_x, k[i], y1, temp);
        gvec_pos(temp, y1);
    }
}

#ifdef ODE_EULER
FUNCTION_ODE_METHOD(euler) {
    LOG_STATS("ode_euler", 1, 0, 0);
    double delta_x = x1 - x0;
    gvec_t k;
    // k = f(x0, y0)
    fun(x0, y0, k, nargs, vargs);
    // y1 = y0 + h * k
    __int_gvec(delta_x, k, y0, y1);
    return true;
}
#endif

FUNCTION_ODE_METHOD(verlet)
{
    LOG_STATS("ode_verlet", 1, 2, 0);
    double delta_x = x1 - x0;
    gvec_t k;
    // k = f(x0, y0)
    fun(x0, y0, k, nargs, vargs);
    // y1 = y0 + h * k
    vec_t foo;
    // k[r] += delta_x * k[v] / 2
    vec_muls(&k[7], 0.5 * delta_x, foo);
    vec_add(&k[0], foo, &k[0]);
    // k[th] += delta_x * k[om] / 2
    vec_muls(&k[10], 0.5 * delta_x, foo);
    vec_add(&k[4], foo, &k[4]);
    __int_gvec(delta_x, k, y0, y1);
    return true;
}

FUNCTION_ODE_METHOD(rk4)
{
    LOG_STATS("ode_rk4", 4, 3, 0);
    static const double A[3][3] = {
        {0.5},
        {0,0, 0.5},
        {0.0, 0.0, 1.0},
    };
    static const double b_bar[4] = {1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0};
    static const double c_bar[3] = {0.5, 0.5, 1.0};
    
    double delta_x = x1 - x0;
    gvec_t k[4], y;
    
    // k[0] = f(x0, y0)
    va_list cargs;
    va_copy(cargs, *vargs);
    fun(x0, y0, k[0], nargs, &cargs);
    va_end(cargs);
    
    // k[1] = f(x0 + h / 2, y0 + h * k[0] / 2)
    // k[2] = f(x0 + h / 2, y0 + h * k[1] / 2)
    // k[3] = f(x0 + h, y0 + h * k[2])
     for (size_t i = 0; i < 3; i++) {
        __apply_stage(i+1, A[i], delta_x, y0, y, k);
        va_copy(cargs, *vargs);
        fun(x0 + c_bar[i] * delta_x, y, k[i+1], nargs, &cargs);
        va_end(cargs);
    }
    
    // y1 = y0 + h * (k[0] / 6 + k[1] / 3 + k[2] / 3 + k[3] / 6)
     __apply_stage(4, b_bar, delta_x, y0, y1, k);
    return true;
}

#ifdef ODE_EULER
FUNCTION_ODE_METHOD(heuler)
{
    LOG_STATS("ode_heuler", 1, 2, 0);
    assert(step != NULL);
    double delta_x = x1 - x0;
    gvec_t k, y2, temp;
    
    // k[0] = f(x0, y0)
    va_list cargs;
    va_copy(cargs, *vargs);
    fun(x0, y0, k, nargs, &cargs);
    va_end(cargs);
    __int_gvec(0.5 * delta_x, k, y0, temp);
    
    // k[1] = f(x0 + h, y0 + h * k[0])
    va_copy(cargs, *vargs);
    __int_gvec(delta_x, k, y0, y1);
    fun(x1, y1, k, nargs, &cargs);
    va_end(cargs);
    __int_gvec(0.5 * delta_x, k, temp, y2);
    
    // y2 = y0 + h * (k[0] + k[1]) / 2
    // y1 = y0 + h * k[0]
    return step(y0, y1, y2, abstol, reltol, 1, nargs, vargs);
}
#endif

FUNCTION_ODE_METHOD(dopri)
{
    LOG_STATS("ode_dopri", 7, 6, 0);
    static const double A[6][6] = {
        {0.2},
        {0.075, 0.225},
        {44.0 / 45.0, -56.0 / 15.0, 32.0 / 9.0},
        {19372.0 / 6561.0, -25360.0 / 2187.0, 64448.0 / 6561.0, -212.0 / 729.0},
        {9017.0 / 3168.0, -355.0 / 33.0, 46732.0 / 5247.0, 49.0 / 176.0, -5103.0 / 18656.0},
        {35.0 / 384.0, 0.0, 500.0 / 1113.0, 125.0 / 192.0, -2187.0 / 6784.0, 11.0 / 84.0}
    };
    static const double b2_bar[7] = {5179.0 / 57600.0, 0.0, 7571.0 / 16695.0, 393.0 / 640.0, -92097.0 / 339200.0, 187.0 / 2100.0, 0.025};
    static const double c_bar[6] = {0.2, 0.3, 0.8, 8.0 / 9.0, 1.0, 1.0};
    
    assert(step != NULL);
    double delta_x = x1 - x0;
    gvec_t k[7], y2;
    
    // k[0] = f(x0, y0)
    va_list cargs;
    va_copy(cargs, *vargs);
    fun(x0, y0, k[0], nargs, &cargs);
    va_end(cargs);
    
    /* k[1] = f(x0 + h / 5, y0 + h * k[0] / 5)
     * k[2] = f(x0 + 3 * h / 10,
     *          y0 + h * (  3 * k[0] / 40
     *                    + 9 * k[1] / 40))
     * k[3] = f(x0 + 4 * h / 5,
     *          y0 + h * (  44 * k[0] / 45
     *                    - 56 * k[1] / 15
     *                    + 32 * k[2] / 9))
     * k[4] = f(x0 + 8 * h / 9,
     *          y0 + h * (  19372 * k[0] / 6561
     *                    - 25360 * k[1] / 2187
     *                    + 64448 * k[2] / 6561
     *                    -   212 * k[3] / 729))
     * k[5] = f(x0 + h,
     *          y0 + h * (   9017 * k[0] / 3168
     *                    -   355 * k[1] / 33
     *                    + 46732 * k[2] / 5247
     *                    +    49 * k[3] / 176
     *                    -  5103 * k[4] / 18656))
     * k[6] = f(x0 + h,
     *          y0 + h * (    35 * k[0] / 384
     *                    +  500 * k[2] / 1113
     *                    +  125 * k[3] / 192
     *                    - 2187 * k[4] / 6784
     *                    +   11 * k[5] / 84))
     */
     for (size_t i = 0; i < 6; i++) {
        __apply_stage(i+1, A[i], delta_x, y0, y1, k);
        va_copy(cargs, *vargs);
        fun(x0 + c_bar[i] * delta_x, y1, k[i+1], nargs, &cargs);
        va_end(cargs);
    }
    

    /* y1 = y0 + h * (    35 * k[0] / 384
     *                +  500 * k[2] / 1113
     *                +  125 * k[3] / 192
     *                - 2187 * k[4] / 6784
     *                +   11 * k[5] / 84))
     * y2 = y0 + h * (   5179 * k[0] / 57600
     *                +  7571 * k[2] / 16695
     *                +   393 * k[3] / 640
     *                - 92097 * k[4] / 339200
     *                +   187 * k[5] / 2100
     *                +         k[6] / 40))
     */
     __apply_stage(7, b2_bar, delta_x, y0, y2, k);
    return step(y0, y1, y2, abstol, reltol, 4, nargs, vargs);
}

#ifdef ODE_EULER
FUNCTION_ODE_METHOD(beuler)
{
    LOG_STATS("ode_beuler", 1, 0, 0);
    double delta_x = x1 - x0;
    gvec_t k, delta_k;
    gvec_zero(k);
    gvec_zero(delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_beuler", 13, 13, 0);
        va_copy(cargs, *vargs);
        fun(x1, y1, delta_k, nargs, &cargs);
        va_end(cargs);
        gvec_sub(delta_k, k, delta_k);
        gvec_add(k, delta_k, k);
        done = true;
        for (size_t i = 0; i < GMAT_NDIM; i++)
            done &= fabs(delta_k[i]) < abstol[i] + reltol[i] * fabs(k[i]);
        __int_gvec(delta_x, k, y0, y1);
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    return true;
}
#endif

FUNCTION_ODE_METHOD(midp)
{
    LOG_STATS("ode_midp", 1, 0, 0);
    double delta_x = x1 - x0, x;
    gvec_t k, delta_k, y;
    gvec_zero(k);
    gvec_zero(delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_midp", 14, 14, 0);
        va_copy(cargs, *vargs);
        x = 0.5 * (x1 + x0);
        __interp_gvec(x0, y0, x1, y1, x, y);
        fun(x, y, delta_k, nargs, &cargs);
        va_end(cargs);
        gvec_sub(delta_k, k, delta_k);
        gvec_add(k, delta_k, k);
        done = true;
        for (size_t i = 0; i < GMAT_NDIM; i++)
            done &= fabs(delta_k[i]) < abstol[i] + reltol[i] * fabs(k[i]);
        __int_gvec(delta_x, k, y0, y1);
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    return true;
}

FUNCTION_ODE_METHOD(vgl4)
{
    LOG_STATS("ode_vgl4", 1, 0, 0);
    static const double A[2][2] = {
        {0.25, 0.25 - 0.5 / sqrt(3.0)},
        {0.25 + 0.5 / sqrt(3.0), 0.25}
    };
    static const double b1_bar[2] = {0.5, 0.5};
    static const double b2_bar[2] = {0.5 * (1.0 + sqrt(3.0)), 0.5 * (1.0 - sqrt(3.0))};
    static const double c_bar[2] = {0.5 * (1.0 - 1.0 / sqrt(3.0)), 0.5 * (1.0 + 1.0 / sqrt(3.0))};
    
    double delta_x = x1 - x0;
    gvec_t k[2], delta_k, y2;
    for (size_t i = 0; i < 2; i++)
        gvec_zero(k[i]);
    gvec_zero(delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_vgl4", 27, 27, 0);
        done = true;
        for (size_t i = 0; i < 2; i++) {
            __apply_stage(2, A[i], delta_x, y0, y2, k);
            va_copy(cargs, *vargs);
            fun(x0 + c_bar[i] * delta_x, y2, delta_k, nargs, &cargs);
            va_end(cargs);
            gvec_sub(delta_k, k[i], delta_k);
            gvec_add(k[i], delta_k, k[i]);
            for (size_t j = 0; j < GMAT_NDIM; j++)
                done &= fabs(delta_k[j]) < abstol[j] + reltol[j] * fabs(k[i][j]);
        }
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    __apply_stage(2, b1_bar, delta_x, y0, y1, k);
    if (step == NULL)
        return true;
    __apply_stage(2, b2_bar, delta_x, y0, y2, k);
    return step(y0, y1, y2, abstol, reltol, 4, nargs, vargs);
}

FUNCTION_ODE_METHOD(vgl6)
{
    LOG_STATS("ode_vgl6", 1, 0, 0);
    static const double A[3][3] = {
        {5.0 / 36.0, 2.0 / 9.0 - 1.0 / sqrt(15.0), 5.0 / 36.0 - 1.0 / sqrt(15.0) / 2.0},
        {5.0 / 36.0 + sqrt(15.0) / 24.0, 2.0 / 9.0, 5.0 / 36.0 - sqrt(15.0) / 24.0},
        {5.0 / 36.0 + 1.0 / sqrt(15.0) / 2.0, 2.0 / 9.0 + 1.0 / sqrt(15.0), 5.0 / 36.0}
    };
    static const double b1_bar[3] = {5.0 / 18.0, 4.0 / 9.0, 5.0 / 18.0};
    static const double b2_bar[3] = {-5.0 / 6.0, 8.0 / 3.0, -5.0 / 6.0};
    static const double c_bar[3] = {(1.0 - sqrt(3.0 / 5.0)) / 2.0, 1.0 / 2.0, (1.0 + sqrt(3.0 / 5.0)) / 2.0};
    
    double delta_x = x1 - x0;
    gvec_t k[3], delta_k, y2;
    for (size_t i = 0; i < 3; i++)
        gvec_zero(k[i]);
    gvec_zero(delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_vgl6", 40, 40, 0);
        done = true;
        for (size_t i = 0; i < 3; i++) {
            __apply_stage(3, A[i], delta_x, y0, y2, k);
            va_copy(cargs, *vargs);
            fun(x0 + c_bar[i] * delta_x, y2, delta_k, nargs, &cargs);
            va_end(cargs);
            gvec_sub(delta_k, k[i], delta_k);
            gvec_add(k[i], delta_k, k[i]);
            for (size_t j = 0; j < GMAT_NDIM; j++)
                done &= fabs(delta_k[j]) < abstol[j] + reltol[j] * fabs(k[i][j]);
        }
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    __apply_stage(3, b1_bar, delta_x, y0, y1, k);
    if (step == NULL)
        return true;
    __apply_stage(3, b2_bar, delta_x, y0, y2, k);
    return step(y0, y1, y2, abstol, reltol, 6, nargs, vargs);
}

bool solve_ivp(
    double x0, const gvec_t y0,
    double x1, gvec_t y1,
    ode_meth_t meth, ode_fun_t fun, ode_step_t step,
    const gvec_t abstol, const gvec_t reltol,
    size_t nargs, ...
) {
    LOG_STATS("solve_ivp", 0, 0, 0);
    if (meth == NULL) meth = ode_default_meth;
    va_list vargs;
    va_start(vargs, nargs);
    bool done = meth(x0, y0, x1, y1, fun, step, abstol, reltol, nargs, &vargs);
    va_end(vargs);
    return done;
}

