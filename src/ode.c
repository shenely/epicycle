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

static void __apply_stage(
    size_t N, const double A[static N],
    double delta_x,
    const st_t* y0,
    st_t* restrict y1,
    const st_t k[static N]
) {
    LOG_STATS("__apply_stage", 0, N, 0);
    st_int(A[0] * delta_x, &k[0], y0, y1);
    for (size_t i = 1; i < N; i++) {
        if (fabs(A[i]) < ABSTOL)
            continue;
        st_t temp;
        st_int(A[i] * delta_x, &k[i], y1, &temp);
        st_pos(&temp, y1);
    }
}

static bool __check_error(
    const st_t* delta_k,
    const st_t* k
) {
    LOG_STATS("__check_error", 0, 0, 0);
    vec_t foo, bar;
    bool done = true;
    done &= vec_norm(delta_k->r_bar) < ABSTOL + RELTOL * vec_norm(k->r_bar);
    quat_log(delta_k->q, foo);
    quat_log(k->q, bar);
    done &= vec_norm(foo) < ABSTOL + RELTOL * vec_norm(bar);
    done &= vec_norm(delta_k->v_bar) < ABSTOL + RELTOL * vec_norm(k->v_bar);
    done &= vec_norm(delta_k->om_bar) < ABSTOL + RELTOL * vec_norm(k->om_bar);
    return done;
}

#ifdef ODE_EULER
FUNCTION_ODE_METHOD(euler) {
    LOG_STATS("ode_euler", 1, 0, 0);
    double delta_x = x1 - x0;
    st_t k;
    // k = f(x0, y0)
    fun(x0, y0, &k, nargs, vargs);
    // y1 = y0 + h * k
    st_int(delta_x, &k, y0, y1);
    return true;
}
#endif

FUNCTION_ODE_METHOD(verlet) {
    LOG_STATS("ode_verlet", 1, 2, 0);
    double delta_x = x1 - x0;
    st_t k;
    // k = f(x0, y0)
    fun(x0, y0, &k, nargs, vargs);
    // y1 = y0 + h * k
    vec_t foo;
    quat_t bar, temp;
    // k[r] += delta_x * k[v] / 2
    vec_muls(k.v_bar, 0.5 * delta_x, foo);
    vec_add(k.r_bar, foo, k.r_bar);
    // k[th] += delta_x * k[om] / 2
    vec_muls(k.om_bar, 0.25 * delta_x, foo);
    vec_exp(foo, bar);
    quat_pos(k.q, temp);
    quat_mul(temp, bar, k.q);
    st_int(delta_x, &k, y0, y1);
    return true;
}

FUNCTION_ODE_METHOD(rk4) {
    LOG_STATS("ode_rk4", 4, 3, 0);
    static const
    double A[3][3] = {
            {0.5          },
            {0,0, 0.5     },
            {0.0, 0.0, 1.0},
        },
        b_bar[4] = {0.16666666666666666, 0.33333333333333333, 0.33333333333333333, 0.16666666666666666},
        c_bar[3] = {0.5, 0.5, 1.0};
    
    double delta_x = x1 - x0;
    st_t k[4], y;
    
    // k[0] = f(x0, y0)
    va_list cargs;
    va_copy(cargs, *vargs);
    fun(x0, y0, &k[0], nargs, &cargs);
    va_end(cargs);
    
    // k[1] = f(x0 + h / 2, y0 + h * k[0] / 2)
    // k[2] = f(x0 + h / 2, y0 + h * k[1] / 2)
    // k[3] = f(x0 + h, y0 + h * k[2])
     for (size_t i = 0; i < 3; i++) {
        __apply_stage(i+1, A[i], delta_x, y0, &y, k);
        va_copy(cargs, *vargs);
        fun(x0 + c_bar[i] * delta_x, &y, &k[i+1], nargs, &cargs);
        va_end(cargs);
    }
    
    // y1 = y0 + h * (k[0] / 6 + k[1] / 3 + k[2] / 3 + k[3] / 6)
     __apply_stage(4, b_bar, delta_x, y0, y1, k);
    return true;
}

#ifdef ODE_EULER
FUNCTION_ODE_METHOD(heuler) {
    LOG_STATS("ode_heuler", 1, 2, 0);
    assert(step != NULL);
    double delta_x = x1 - x0;
    st_t k, y2, temp;
    
    // k[0] = f(x0, y0)
    va_list cargs;
    va_copy(cargs, *vargs);
    fun(x0, y0, k, nargs, &cargs);
    va_end(cargs);
    st_int(0.5 * delta_x, &k, y0, &temp);
    
    // k[1] = f(x0 + h, y0 + h * k[0])
    va_copy(cargs, *vargs);
    st_int(delta_x, &k, y0, y1);
    fun(x1, y1, &k, nargs, &cargs);
    va_end(cargs);
    st_int(0.5 * delta_x, &k, &temp, &y2);
    
    // y2 = y0 + h * (k[0] + k[1]) / 2
    // y1 = y0 + h * k[0]
    return step(y0, y1, &y2, 1, nargs, vargs);
}
#endif

FUNCTION_ODE_METHOD(dopri) {
    LOG_STATS("ode_dopri", 7, 6, 0);
    static const
    double A[6][6] = {
            {+0.2                },
            {+0.075              , +0.225             },
            {+0.9777777777777777 , -3.7333333333333334, +3.5555555555555554 },
            {+2.9525986892242035 , -11.595793324188385, +9.822892851699436  , -0.2908093278463649},
            {+2.8462752525252526 , -10.757575757575758, +8.906422717743473  , +0.2784090909090909, -0.2735313036020583},
            {+0.09114583333333333,  0.0               , +0.44923629829290207, +0.6510416666666666, -0.322376179245283 , +0.13095238095238096}
        },
        b2_bar[7] = {+0.08991319444444444,  0.0, +0.4534890685834082, +0.6140625         , -0.2715123820754717, +0.08904761904761904, +0.025},
        c_bar[6]  = {+0.2                , +0.3, +0.8               , +0.8888888888888888, +1.0               , +1.0                };
    
    assert(step != NULL);
    double delta_x = x1 - x0;
    st_t k[7], y2;
    
    // k[0] = f(x0, y0)
    va_list cargs;
    va_copy(cargs, *vargs);
    fun(x0, y0, &k[0], nargs, &cargs);
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
        fun(x0 + c_bar[i] * delta_x, y1, &k[i+1], nargs, &cargs);
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
     __apply_stage(7, b2_bar, delta_x, y0, &y2, k);
    return step(y0, y1, &y2, 4, nargs, vargs);
}

#ifdef ODE_EULER
FUNCTION_ODE_METHOD(beuler) {
    LOG_STATS("ode_beuler", 1, 0, 0);
    double delta_x = x1 - x0;
    st_t k, delta_k;
    st_zero(&k);
    st_zero(&delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_beuler", 13, 13, 0);
        va_copy(cargs, *vargs);
        fun(x1, y1, &delta_k, nargs, &cargs);
        va_end(cargs);
        st_sub(&delta_k, &k, &delta_k);
        st_add(&k, &delta_k, &k);
        done = __check_error(&delta_k, &k);
        st_int(delta_x, &k, y0, y1);
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    return true;
}
#endif

FUNCTION_ODE_METHOD(midp) {
    LOG_STATS("ode_midp", 1, 0, 0);
    double delta_x = x1 - x0, x;
    st_t k, delta_k, y;
    st_zero(&k);
    st_zero(&delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_midp", 14, 14, 0);
        va_copy(cargs, *vargs);
        x = 0.5 * (x1 + x0);
        st_interp(x0, y0, x1, y1, x, &y);
        fun(x, &y, &delta_k, nargs, &cargs);
        va_end(cargs);
        st_sub(&delta_k, &k, &delta_k);
        st_add(&k, &delta_k, &k);
        done = __check_error(&delta_k, &k);
        st_int(delta_x, &k, y0, y1);
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    return true;
}

FUNCTION_ODE_METHOD(vgl4) {
    LOG_STATS("ode_vgl4", 1, 0, 0);
    static const
    double A[2][2] = {
            {+0.25              , -0.03867513459481292},
            {+0.5386751345948129, +0.25               }
        },
        b1_bar[2] = {+0.5                , +0.5               },
        b2_bar[2] = {+1.3660254037844386 , -0.3660254037844386},
        c_bar[2]  = {+0.21132486540518708, +0.7886751345948129};
    
    double delta_x = x1 - x0;
    st_t k[2], delta_k, y2;
    for (size_t i = 0; i < 2; i++)
        st_zero(&k[i]);
    st_zero(&delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_vgl4", 27, 27, 0);
        done = true;
        for (size_t i = 0; i < 2; i++) {
            __apply_stage(2, A[i], delta_x, y0, &y2, k);
            va_copy(cargs, *vargs);
            fun(x0 + c_bar[i] * delta_x, &y2, &delta_k, nargs, &cargs);
            va_end(cargs);
            st_sub(&delta_k, &k[i], &delta_k);
            st_add(&k[i], &delta_k, &k[i]);
            done &= __check_error(&delta_k, &k[i]);
        }
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    __apply_stage(2, b1_bar, delta_x, y0, y1, k);
    if (step == NULL)
        return true;
    __apply_stage(2, b2_bar, delta_x, y0, &y2, k);
    return step(y0, y1, &y2, 4, nargs, vargs);
}

FUNCTION_ODE_METHOD(vgl6) {
    LOG_STATS("ode_vgl6", 1, 0, 0);
    static const
    double A[3][3] = {
            {+0.1388888888888889 , -0.03597666752493889, +0.009789444015308346},
            {+0.3002631949808646 , +0.2222222222222222 , -0.022485417203086805},
            {+0.26798833376246944, +0.4804211119693833 , +0.1388888888888889  }
        },
        b1_bar[3] = {+0.2777777777777778, +0.4444444444444444, +0.2777777777777778},
        b2_bar[3] = {-0.8333333333333334, +2.6666666666666665, -0.8333333333333334},
        c_bar[3]  = {+0.1127016653792583, +0.5               , +0.8872983346207417};
    
    double delta_x = x1 - x0;
    st_t k[3], delta_k, y2;
    for (size_t i = 0; i < 3; i++)
        st_zero(&k[i]);
    st_zero(&delta_k);
    
    va_list cargs;
    bool done = false;
    for (size_t n = 0; n < MAXITER; n++) {
        LOG_STATS("ode_vgl6", 40, 40, 0);
        done = true;
        for (size_t i = 0; i < 3; ++i) {
            __apply_stage(3, A[i], delta_x, y0, &y2, k);
            va_copy(cargs, *vargs);
            fun(x0 + c_bar[i] * delta_x, &y2, &delta_k, nargs, &cargs);
            va_end(cargs);
            st_sub(&delta_k, &k[i], &delta_k);
            st_add(&k[i], &delta_k, &k[i]);
            done &= __check_error(&delta_k, &k[i]);
        }
        if (n < 2) continue;
        else if (done) break;
    }
    assert(done);
    __apply_stage(3, b1_bar, delta_x, y0, y1, k);
    if (step == NULL)
        return true;
    __apply_stage(3, b2_bar, delta_x, y0, &y2, k);
    return step(y0, y1, &y2, 6, nargs, vargs);
}

bool
solve_ivp(
    double x0, const st_t* y0,
    double x1, st_t* restrict y1,
    ode_meth_t meth, ode_fun_t fun, ode_step_t step,
    size_t nargs, ...
) {
    LOG_STATS("solve_ivp", 0, 0, 0);
    if (meth == NULL) meth = ode_default_meth;
    va_list vargs;
    va_start(vargs, nargs);
    bool done = meth(x0, y0, x1, y1, fun, step, nargs, &vargs);
    va_end(vargs);
    return done;
}

