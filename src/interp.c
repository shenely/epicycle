#include <assert.h>
#include <math.h>
#include "interp.h"
#include "poly.h"
#include "log.h"

static struct poly_s
    /* cubic Hermite basis functions */
    __herm3_h00 = {.deg=3, .coeff={+1.0,  0.0, -3.0, +2.0}},
    __herm3_h10 = {.deg=3, .coeff={ 0.0, +1.0, -2.0, +1.0}},
    __herm3_h01 = {.deg=3, .coeff={ 0.0,  0.0, +3.0, -2.0}},
    __herm3_h11 = {.deg=3, .coeff={ 0.0,  0.0, -1.0, +1.0}},
    __herm3_g00 = {.deg=2, .coeff={0.0, 0.0, 0.0}},
    __herm3_g10 = {.deg=2, .coeff={0.0, 0.0, 0.0}},
    __herm3_g01 = {.deg=2, .coeff={0.0, 0.0, 0.0}},
    __herm3_g11 = {.deg=2, .coeff={0.0, 0.0, 0.0}},
    /* cubic Bezier basis functions */
    __herm3_b1 = {.deg=3, .coeff={ 0.0, +3.0, -3.0, +1.0}},
    __herm3_b2 = {.deg=3, .coeff={ 0.0,  0.0, +3.0, -2.0}},
    __herm3_b3 = {.deg=3, .coeff={ 0.0,  0.0,  0.0, +1.0}},
    __herm3_c1 = {.deg=2, .coeff={0.0, 0.0, 0.0}},
    __herm3_c2 = {.deg=2, .coeff={0.0, 0.0, 0.0}},
    __herm3_c3 = {.deg=2, .coeff={0.0, 0.0, 0.0}};
    
void interp_init() {
    LOG_STATS("interp_init", 0, 0, 0);
    poly_diff(&__herm3_h00, &__herm3_g00);
    poly_diff(&__herm3_h10, &__herm3_g10);
    poly_diff(&__herm3_h01, &__herm3_g01);
    poly_diff(&__herm3_h11, &__herm3_g11);
    poly_diff(&__herm3_b1, &__herm3_c1);
    poly_diff(&__herm3_b2, &__herm3_c2);
    poly_diff(&__herm3_b3, &__herm3_c3);
}

/* Linear interpolation */
double interp_lerp(
    double x0, double y0,
    double x1, double y1,
    double x
) {
    LOG_STATS("interp_lerp", 4, 3, 0);
    // t = (x - x[0] / (x[1] - x[0])
    double delta_x = x1 - x0,
           t = (x - x0) / delta_x;
    // y(x) = y[0] * (1 - t) + y[1] * t
    return y0 * (1.0 - t) + y1 * t;
}

/* Vector linear interpolation */
void interp_vlerp(
    double x0, const vec_t* const v0_bar,
    double x1, const vec_t* const v1_bar,
    double x, vec_t* restrict v_bar
) {
    LOG_STATS("interp_vlerp", 2, 1, 0);
    // t = (x - x[0] / (x[1] - x[0])
    double delta_x = x1 - x0,
           t = (x - x0) / delta_x;
    // v(x) = v[0] * (1 - t) + v[1] * t
    vec_sub(v1_bar, v0_bar, v_bar);
    vec_muls(v_bar, t, v_bar);
    vec_add(v_bar, v0_bar, v_bar);
}

/* Spherical linear interpolation */
void interp_slerp(
    double x0, const quat_t* q0,
    double x1, const quat_t* q1,
    double x, quat_t* restrict q
) {
    LOG_STATS("interp_slerp", 2, 1, 0);
    assert((q0 != q) && (q1 != q));
    // t = (x - x[0] / (x[1] - x[0])
    double delta_x = x1 - x0,
           t = (x - x0) / delta_x;
    quat_t temp;
    quat_conj(q0, &temp);
    quat_mul(&temp, q1, q);
    quat_pow(q, t, &temp);
    quat_mul(q0, &temp, q);
}

/* Cublic spline interpolation */
void interp_cint(
    double x0, const vec_t* p0_bar, const vec_t* m0_bar,
    double x1, const vec_t* p1_bar, const vec_t* m1_bar,
    double x, vec_t* restrict p_bar, vec_t* restrict m_bar
) {
    LOG_STATS("interp_cint", 2, 2, 0);
    assert(
        (p0_bar != p_bar)
        && (p1_bar != p_bar)
        && (m0_bar != m_bar)
        && (m1_bar != m_bar)
    );
    // t = (x - x[0] / (x[1] - x[0])
    double delta_x = x1 - x0,
           t = (x - x0) / delta_x;
    
    // setup temperatory variables
    vec_t temp;
    
    // p(x) = h[0,0](t) * p[0]
    //      + h[1,0](t) * m[0]
    //      + h[0,1](t) * p[1]
    //      + h[1,1](t) * m[1]
    vec_muls(m0_bar, poly_eval(&__herm3_h10, t), p_bar);
    vec_muls(m1_bar, poly_eval(&__herm3_h11, t), &temp);
    vec_add(p_bar, &temp, p_bar);
    vec_muls(p_bar, delta_x, p_bar);
    vec_muls(p0_bar, poly_eval(&__herm3_h00, t), &temp);
    vec_add(p_bar, &temp, p_bar);
    vec_muls(p1_bar, poly_eval(&__herm3_h01, t), &temp);
    vec_add(p_bar, &temp, p_bar);
    
    // m(x) = d{h[0,0](t)}/dt * p[0]
    //      + d{h[1,0](t)}/dt * m[0]
    //      + d{h[0,1](t)}/dt * p[1]
    //      + d{h[1,1](t)}/dt * m[1]
    vec_muls(p0_bar, poly_eval(&__herm3_g00, t), m_bar);
    vec_muls(p1_bar, poly_eval(&__herm3_g01, t), &temp);
    vec_add(m_bar, &temp, m_bar);
    vec_muls(m_bar, 1.0 / delta_x, m_bar);
    vec_muls(m0_bar, poly_eval(&__herm3_g10, t), &temp);
    vec_add(m_bar, &temp, m_bar);
    vec_muls(m1_bar, poly_eval(&__herm3_g11, t), &temp);
    vec_add(m_bar, &temp, m_bar);
}

/* Spherical cubic interpolation */
void interp_squad(
    double x_a, const quat_t* q_a, const vec_t* om_a,
    double x_b, const quat_t* q_b, const vec_t* om_b,
    double x, quat_t* restrict q, vec_t* restrict om
) {
    LOG_STATS("interp_squad", 2, 4, 0);
    assert(
        (q_a != q)
        && (q_a != q)
        && (om_a != om)
        && (om_b != om)
    );
    // t = (x - x[0] / (x[1] - x[0])
    double delta_x = x_b - x_a,
           t = (x - x_a) / delta_x;
    vec_t om1, om2, om3, temp;
    quat_t q1, q2, q3, foo, bar;
           
    // intermediate angular velocities
    vec_muls(om_a, delta_x / 6.0, &om1);
    vec_muls(om_b, delta_x / 6.0, &om3);
    
    // intermediate quaternions
    vec_exp(&om1, &q1);
    vec_exp(&om3, &q3);
    quat_mul(q_a, &q1, &q2);
    quat_conj(&q2, &q2);
    quat_mul(&q2, q_b, &foo);
    quat_conj(&q3, &bar);
    quat_mul(&foo, &bar, &q2);
    quat_log(&q2, &om2);
    vec_muls(&om1, poly_eval(&__herm3_b1, t), &temp);
    vec_exp(&temp, &q1);
    vec_muls(&om2, poly_eval(&__herm3_b2, t), &temp);
    vec_exp(&temp, &q2);
    vec_muls(&om3, poly_eval(&__herm3_b3, t), &temp);
    vec_exp(&temp, &q3);
    
    // q(x) = q[0] * exp(om[1] * b[1](t))
    //             * exp(om[2] * b[2](t))
    //             * exp(om[3] * b[3](t))
    quat_mul(q_a, &q1, &foo);
    quat_mul(&foo, &q2, &bar);
    quat_mul(&bar, &q3, q);

    // d{q(x)}/dt = q[0] * exp(om[1] * b[1](t)) * (om[1] * d{b[1](x)}/dt)
    //                   * exp(om[2] * b[2](t))
    //                   * exp(om[3] * b[3](t))
    //            + q[0] * exp(om[1] * b[1](t))
    //                   * exp(om[2] * b[2](t)) * (om[2] * d{b[2](x)}/dt)
    //                   * exp(om[3] * b[3](t))
    //            + q[0] * exp(om[1] * b[1](t))
    //                   * exp(om[2] * b[2](t))
    //                   * exp(om[3] * b[3](t)) * (om[3] * d{b[3](x)}/dt)
    //            = 0.5 * q(t) * om(t)
    //            = 0.5 * q[0] * exp(om[1] * b[1](t))
    //                         * exp(om[2] * b[2](t))
    //                         * exp(om[3] * b[3](t))
    //                         * om(t)
    // om(t) = exp(- om[3] * b[1](t))
    //       * exp(- om[2] * b[2](t))
    //       * (om[1] * d{b[1](x)}/dt)
    //       * exp(om[2] * b[2](t))
    //       * exp(om[3] * b[3](t))
    //       + exp(- om[3] * b[1](t))
    //       * (om[2] * d{b[2](x)}/dt)
    //       * exp(om[3] * b[3](t))
    //       + (om[3] * d{b[3](x)}/dt)
    vec_muls(&om1, poly_eval(&__herm3_c1, t), &om1);
    vec_irot(&q2, &om1, &temp);
    vec_muls(&om2, poly_eval(&__herm3_c2, t), &om2);
    vec_add(&om2, &temp, &temp);
    vec_irot(&q3, &temp, om);
    vec_muls(&om3, poly_eval(&__herm3_c3, t), &om3);
    vec_add(&om3, om, om);
    vec_muls(om, 2.0 / delta_x, om);
}

