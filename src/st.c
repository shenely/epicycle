#include "st.h"
#include "interp.h"
#include "config.h"
#include "log.h"

void
st_zero(
    st_t* restrict a
) {
    LOG_STATS("st_zero", 0, 0, 0);
    vec_zero(a->r_bar);
    quat_one(a->q);
    vec_zero(a->v_bar);
    vec_zero(a->om_bar);
}

void
st_neg(
    const st_t* a,
    st_t* b
) {
    LOG_STATS("st_neg", 0, 0, 0);
    vec_neg(a->r_bar, b->r_bar);
    quat_conj(a->q, b->q);
    vec_neg(a->v_bar, b->v_bar);
    vec_neg(a->om_bar, b->om_bar);
}

void
st_add(
    const st_t* a,
    const st_t* b,
    st_t* c
) {
    LOG_STATS("st_add", 0, 0, 0);
    quat_t temp;
    vec_add(a->r_bar, b->r_bar, c->r_bar);
    quat_mul(a->q, b->q, temp);
    quat_pos(temp, c->q);
    vec_add(a->v_bar, b->v_bar, c->v_bar);
    vec_add(a->om_bar, b->om_bar, c->om_bar);
}

void
st_sub(
    const st_t* a,
    const st_t* b,
    st_t* c
) {
    LOG_STATS("st_sub", 0, 0, 0);
    quat_t foo, bar;
    vec_sub(a->r_bar, b->r_bar, c->r_bar);
    quat_conj(b->q, foo);
    quat_mul(foo, a->q, bar);
    quat_pos(bar, c->q);
    vec_sub(a->v_bar, b->v_bar, c->v_bar);
    vec_sub(a->om_bar, b->om_bar, c->om_bar);
}

void 
st_muls(
    const st_t* a,
    double s,
    st_t* b
) {
    LOG_STATS("st_muls", 0, 0, 0);
    vec_muls(a->r_bar, s, b->r_bar);
    quat_pow(a->q, s, b->q);
    vec_muls(a->v_bar, s, b->v_bar);
    vec_muls(a->om_bar, s, b->om_bar);
}


void st_interp(
    double x0, const st_t* y0,
    double x1, const st_t* y1,
    double x, st_t* restrict y
) {
    LOG_STATS("st_interp", 0, 0, 0);
    interp_vlerp(x0, y0->r_bar, x1, y1->r_bar, x, y->r_bar);
    interp_slerp(x0, y0->q, x1, y1->q, x, y->q);
    interp_vlerp(x0, y0->v_bar, x1, y1->v_bar, x, y->v_bar);
    interp_vlerp(x0, y0->om_bar, x1, y1->om_bar, x, y->om_bar);
}

void st_int(
    double delta_x,
    const st_t* k,
    const st_t* y0,
    st_t* restrict y1
) {
    LOG_STATS("st_int", 0, 0, 0);
    vec_t foo;
    quat_t bar;
    // r1_bar = r0_bar + delta_x * v_bar
    vec_muls(k->r_bar, delta_x, foo);
    vec_add(y0->r_bar, foo, y1->r_bar);
    // q1 = q0 * exp(om_bar / 2) ** delta_x
    quat_pow(k->q, delta_x, bar);
    quat_mul(y0->q, bar, y1->q);
    // v1_bar = v0_bar + delta_x * v_dot
    vec_muls(k->v_bar, delta_x, foo);
    vec_add(y0->v_bar, foo, y1->v_bar);
    // om1_bar = om0_bar + delta_x * om_dot
    vec_muls(k->om_bar, delta_x, foo);
    vec_add(y0->om_bar, foo, y1->om_bar);
}

