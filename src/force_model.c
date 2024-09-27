#include <assert.h>
#include <math.h>
#include <string.h>
#include "force_model.h"
#include "util.h"
#include "mat.h"
#include "dmat.h"
#include "quat.h"
#include "interp.h"
#include "log.h"

void interp_st(
    size_t size,
    const struct st_s* prev,
    const struct st_s* next,
    struct st_s* restrict curr
) {
    LOG_STATS("interp_st", 0, 0, 0);
    interp_cint(
        prev->clk.t, prev->sys.r_bar, prev->sys.v_bar,
        next->clk.t, next->sys.r_bar, next->sys.v_bar,
        curr->clk.t, curr->sys.r_bar, curr->sys.v_bar
    );
    interp_squad(
        prev->clk.t, prev->sys.q, prev->sys.om_bar,
        next->clk.t, next->sys.q, next->sys.om_bar,
        curr->clk.t, curr->sys.q, curr->sys.om_bar
    );
    interp_obj_lst(size, prev, next, curr);
}

void interp_obj_lst(
    size_t size,
    const struct st_s* prev,
    const struct st_s* next,
    struct st_s* restrict curr
) {
    LOG_STATS("interp_obj_lst", 0, size, 0);
    for (size_t idx = 0; idx < size; idx++) {
        curr->obj_lst[idx].m = interp_lerp(
            prev->clk.t, prev->obj_lst[idx].m,
            next->clk.t, next->obj_lst[idx].m,
            curr->clk.t
        );
        dmat_muls(
            prev->obj_lst[idx].I_cm,
            curr->obj_lst[idx].m / prev->obj_lst[idx].m,
            curr->obj_lst[idx].I_cm
        );
        /* XXX momentum vectors not interpolated
         *  interp_vlerp(
         *      prev->clk.t, prev->obj_lst[idx].p_bar,
         *      next->clk.t, next->obj_lst[idx].p_bar,
         *      curr->clk.t, curr->obj_lst[idx].p_bar
         *  );
         *  interp_vlerp(
         *      prev->clk.t, prev->obj_lst[idx].h_bar,
         *      next->clk.t, next->obj_lst[idx].h_bar,
         *      curr->clk.t, curr->obj_lst[idx].h_bar
         *  );
         */
        vec_pos(next->obj_lst[idx].p_bar, curr->obj_lst[idx].p_bar);
        vec_pos(next->obj_lst[idx].h_bar, curr->obj_lst[idx].h_bar);
    }
}

void solve_st_dot(
    size_t size,
    const struct cfg_s* cfg,
    const struct st_s* prev,
    struct st_s* restrict next,
    const struct in_s* in
) {
    LOG_STATS("solve_st_dot", 1 + size, 1 + 2 * size, 0);
    double delta_t = cfg->clk.delta_t;
    vec_t foo;
    quat_t bar;
    next->clk.t = prev->clk.t + delta_t;
    vec_muls(prev->sys.v_bar, delta_t, foo);
    vec_add(prev->sys.r_bar, foo, next->sys.r_bar);
    vec_muls(prev->sys.om_bar, 0.5 * delta_t, foo);
    vec_exp(foo, bar);
    quat_mul(prev->sys.q, bar, next->sys.q);
    vec_pos(prev->sys.v_bar, next->sys.v_bar);
    vec_pos(prev->sys.om_bar, next->sys.om_bar);
    for (size_t idx = 0; idx < size; idx++) {
        next->obj_lst[idx].m = (
            prev->obj_lst[idx].m
            + in->obj_lst[idx].m_dot * delta_t
        );
        dmat_muls(
            prev->obj_lst[idx].I_cm,
            next->obj_lst[idx].m / prev->obj_lst[idx].m,
            next->obj_lst[idx].I_cm
        );
        vec_pos(prev->obj_lst[idx].p_bar, next->obj_lst[idx].p_bar);
        vec_pos(prev->obj_lst[idx].h_bar, next->obj_lst[idx].h_bar);
    }
}

void solve_st_delta(
    size_t size,
    const struct cfg_s* cfg,
    const struct st_s* prev,
    struct st_s* restrict next,
    struct out_s* restrict out
) {
    LOG_STATS("solve_st_delta", 0, 1, 0);
    vec_t p_bar, h_bar, temp;
    solve_out(size, cfg, prev, out);
    vec_irot(prev->sys.q, prev->sys.v_bar, p_bar);
    vec_cross(out->sys.c_bar, prev->sys.om_bar, temp);
    vec_sub(p_bar, temp, p_bar);
    vec_muls(p_bar, out->sys.m, p_bar);
    mat_mulv((void*) out->sys.I_cm, prev->sys.om_bar, h_bar);
    vec_cross(out->sys.c_bar, p_bar, temp);
    vec_add(h_bar, temp, h_bar);
    for (size_t idx = 0; idx < size; idx++) {
        vec_t foo, bar;
        vec_sub(next->obj_lst[idx].p_bar, prev->obj_lst[idx].p_bar, temp);
        vec_rot(cfg->obj_lst[idx].q, temp, foo);
        vec_sub(p_bar, foo, p_bar);
        vec_sub(next->obj_lst[idx].h_bar, prev->obj_lst[idx].h_bar, temp);
        vec_rot(cfg->obj_lst[idx].q, temp, bar);
        vec_cross(cfg->obj_lst[idx].r_bar, foo, temp);
        vec_add(bar, temp, bar);
        vec_sub(h_bar, bar, h_bar);
    }
    mat_t eye;
    solve_out(size, cfg, next, out);
    mat_inv((void*) out->sys.I_cm, eye);
    vec_cross(out->sys.c_bar, p_bar, temp);
    vec_sub(h_bar, temp, h_bar);
    mat_mulv((void*) eye, h_bar, next->sys.om_bar);
    vec_cross(out->sys.c_bar, next->sys.om_bar, temp);
    vec_muls(p_bar, 1.0 / out->sys.m, p_bar);
    vec_add(p_bar, temp, p_bar);
    vec_rot(next->sys.q, p_bar, next->sys.v_bar);
}

bool solve_ch(
    size_t size,
    const struct cfg_s* cfg,
    struct ch_s* restrict ch,
    const struct st_s* prev,
    struct st_s* restrict next,
    struct in_s* restrict in,
    struct em_s* restrict em
) {
    bool flag = false;
    for (size_t idx = 0; idx < size; idx++) {
        switch (ch->obj_lst[idx].T) {
        case E_ST:
            LOG_WARNING("[E_ST] `%s`", cfg->obj_lst[idx].sym);
            LOG_STATS("solve_ch[E_ST]", 2, 1, 0);
            flag = true;
            next->obj_lst[idx].m = prev->obj_lst[idx].m
                                 + ch->obj_lst[idx].u.st.m;
            dmat_muls(prev->obj_lst[idx].I_cm,
                      next->obj_lst[idx].m / (next->obj_lst[idx].m - ch->obj_lst[idx].u.st.m),
                      next->obj_lst[idx].I_cm);
            vec_add(prev->obj_lst[idx].p_bar,
                    ch->obj_lst[idx].u.st.p_bar, 
                    next->obj_lst[idx].p_bar);
            vec_add(prev->obj_lst[idx].h_bar,
                    ch->obj_lst[idx].u.st.h_bar,
                    next->obj_lst[idx].h_bar);
            break;
        case E_IN:
            LOG_WARNING("[E_IN] `%s`", cfg->obj_lst[idx].sym);
            LOG_STATS("solve_ch[E_IN]", 0, 0, 0);
            flag = true;
            in->obj_lst[idx].m_dot = ch->obj_lst[idx].u.in.m_dot;
            vec_pos(ch->obj_lst[idx].u.in.F_bar,
                    in->obj_lst[idx].F_bar);
            vec_pos(ch->obj_lst[idx].u.in.M_bar,
                    in->obj_lst[idx].M_bar);
            break;
        case E_EM:
            LOG_WARNING("[E_EM] `%s`", cfg->obj_lst[idx].sym);
            LOG_STATS("solve_ch[E_EM]", 0, 0, 0);
            flag = true;
            em->obj_lst[idx].q = ch->obj_lst[idx].u.em.q;
            vec_pos(ch->obj_lst[idx].u.em.p_bar,
                    em->obj_lst[idx].p_bar);
            vec_pos(ch->obj_lst[idx].u.em.m_bar,
                    em->obj_lst[idx].m_bar);
            break;
        default:
            break;
        }
        ch->obj_lst[idx].T = E_NA;
    }
    return flag;
}

void solve_in(
    size_t size,
    const struct cfg_s* cfg,
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out
) {
    LOG_STATS("solve_in", 0, 1, 0);
    vec_t temp, foo, bar;
    mat_t eye;
    vec_irot(st->sys.q, in->sys.F_bar, temp);
    for (size_t idx = 0; idx < size; idx++) {
        vec_rot(cfg->obj_lst[idx].q, in->obj_lst[idx].F_bar, foo);
        vec_add(temp, foo, temp);
        vec_cross(cfg->obj_lst[idx].r_bar, foo, bar);
        vec_add(in->sys.M_bar, bar, in->sys.M_bar);
        vec_rot(cfg->obj_lst[idx].q, in->obj_lst[idx].M_bar, foo);
        vec_add(in->sys.M_bar, foo, in->sys.M_bar);
    }
    // om_bar
    vec_cross(out->sys.c_bar, temp, foo);
    vec_sub(in->sys.M_bar, foo, in->sys.om_dot);
    mat_mulv(out->sys.I_cm, st->sys.om_bar, foo);
    vec_cross(st->sys.om_bar, foo, bar);
    vec_sub(in->sys.om_dot, bar, foo);
    mat_inv((void*)out->sys.I_cm, eye);
    mat_mulv((void*)eye, foo, in->sys.om_dot);
    // v_bar
    vec_rot(st->sys.q, temp, in->sys.F_bar);
    vec_cross(st->sys.om_bar, out->sys.c_bar, foo);
    vec_cross(st->sys.om_bar, foo, bar);
    vec_cross(out->sys.c_bar, in->sys.om_dot, foo);
    vec_sub(foo, bar, foo);
    vec_rot(st->sys.q, foo, in->sys.v_dot);
    vec_muls(in->sys.F_bar, 1.0 / out->sys.m, foo);
    vec_add(in->sys.v_dot, foo, in->sys.v_dot);
}

void solve_out(
    size_t size,
    const struct cfg_s* cfg,
    const struct st_s* st,
    struct out_s* restrict out
) {
    LOG_STATS("solve_out", 0, 1, 0);
    mat_t eye, temp;
    out->sys.m = 0.0;
    vec_zero(out->sys.c_bar);
    mat_zero(out->sys.I_cm);
    for (size_t idx = 0; idx < size; idx++) {
        vec_t foo;
        mat_t bar;
        out->sys.m += st->obj_lst[idx].m;
        vec_muls(cfg->obj_lst[idx].r_bar, st->obj_lst[idx].m, foo);
        vec_add(out->sys.c_bar, foo, out->sys.c_bar);
        quat_rot_mat(cfg->obj_lst[idx].q, temp);
        dmat_muld((void*)temp, st->obj_lst[idx].I_cm, bar);
        mat__T((void*)temp, temp);
        mat_mul((void*)bar, (void*)temp, eye);
        mat_add((void*)out->sys.I_cm, (void*)eye, out->sys.I_cm);
        vec_cross_mat(cfg->obj_lst[idx].r_bar, temp);
        mat_mul((void*)temp, (void*)temp, eye);
        mat_muls((void*)eye, st->obj_lst[idx].m, eye);
        mat_sub((void*)out->sys.I_cm, (void*)eye, out->sys.I_cm);
    }
    vec_muls(out->sys.c_bar, 1.0 / out->sys.m, out->sys.c_bar);
    vec_cross_mat(out->sys.c_bar, temp);
    mat_mul((void*)temp, (void*)temp, eye);
    mat_muls((void*)eye, out->sys.m, eye);
    mat_add((void*)out->sys.I_cm, (void*)eye, out->sys.I_cm);
}

void solve_em(
    size_t size,
    const struct cfg_s* cfg,
    struct em_s* restrict em
) {
    LOG_STATS("solve_em", 0, 0, 0);
    vec_t temp;
    em->sys.q = 0;
    vec_zero(em->sys.p_bar);
    vec_zero(em->sys.m_bar);
    for (size_t idx = 0; idx < size; idx++) {
        em->sys.q += em->obj_lst[idx].q;
        vec_muls(cfg->obj_lst[idx].r_bar, em->obj_lst[idx].q, temp);
        vec_add(em->sys.p_bar, temp, em->sys.p_bar);
        vec_rot(cfg->obj_lst[idx].q, em->obj_lst[idx].p_bar, temp);
        vec_add(em->sys.p_bar, temp, em->sys.p_bar);
        vec_rot(cfg->obj_lst[idx].q, em->obj_lst[idx].m_bar, temp);
        vec_add(em->sys.m_bar, temp, em->sys.m_bar);
    }
}

bool em(
    size_t size __attribute__((unused)),
    const struct cfg_s* cfg __attribute__((unused)),
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out __attribute__((unused)),
    struct em_s* restrict em
) {
    LOG_STATS("em", 0, 0, 0);
    vec_t foo, bar;
    vec_cross(st->sys.v_bar, em->sys.B_bar, foo);
    vec_add(em->sys.E_bar, foo, foo);
    vec_muls(foo, em->sys.q, foo);
    vec_add(in->sys.F_bar, foo, in->sys.F_bar);
    vec_rot(st->sys.q, em->sys.p_bar, foo);
    vec_cross(foo, em->sys.E_bar, bar);
    vec_add(in->sys.M_bar, bar, in->sys.M_bar);
    vec_rot(st->sys.q, em->sys.m_bar, foo);
    vec_cross(foo, em->sys.B_bar, bar);
    vec_add(in->sys.M_bar, bar, in->sys.M_bar);
    return true;
}

void apply_force_model(
    double x, const gvec_t y, gvec_t f,
    size_t nargs, va_list* vargs
) {
    LOG_STATS("apply_force_model", 0, 0, 0);
    assert(nargs >= 9);
    size_t size                = va_arg(*vargs, size_t);
    const struct cfg_s* cfg    = va_arg(*vargs, void*);
    const struct st_s* prev    = va_arg(*vargs, void*);
    const struct st_s* next    = va_arg(*vargs, void*);
    struct st_s* restrict st   = va_arg(*vargs, void*);
    struct in_s* restrict in   = va_arg(*vargs, void*);
    struct out_s* restrict out = va_arg(*vargs, void*);
    struct em_s* restrict em   = va_arg(*vargs, void*);
    const struct force_model_s* force_model = va_arg(*vargs, void*);
    // copy `st_s` from input `g_vec`
    st->clk.t = x;
    memcpy(&st->sys, y, sizeof(gvec_t));
    interp_obj_lst(size, prev, next, st);
    // XXX reset force/torque first
    vec_zero(in->sys.F_bar);
    vec_zero(in->sys.M_bar);
    vec_zero(em->sys.E_bar);
    vec_zero(em->sys.B_bar);
    solve_out(size, cfg, st, out);
    for (size_t idx = 0; idx < force_model->size; idx++) {
        if (force_model->fun_lst[idx] == NULL)
            continue;
        force_model->fun_lst[idx](size, cfg, st, in, out, em);
    }
    solve_in(size, cfg, st, in, out);
    // copy `in_s` from output `g_vec`
    vec_pos(st->sys.v_bar, &f[0]);
    vec_pos(st->sys.om_bar, &f[4]);
    vec_pos(in->sys.v_dot, &f[7]);
    vec_pos(in->sys.om_dot, &f[10]);
}

bool adjust_time_step(
    const gvec_t y0,
    const gvec_t y1,
    const gvec_t y2,
    const gvec_t abstol __attribute__((unused)),
    const gvec_t reltol __attribute__((unused)),
    int q, size_t nargs, va_list* vargs
) {
    LOG_STATS("adjust_time_step", 2 * GMAT_NDIM, 2 + GMAT_NDIM, 1);
    double E = ABSTOL;
    for (size_t i = 0; i < GMAT_NDIM; i++) {
        double err = fabs(y1[i] - y2[i]),
               tol = abstol[i] + reltol[i] * MAX(fabs(y0[i]), fabs(y1[i]));
        E = MAX(E, err / tol);
    }
    assert(nargs >= 2);  // we only care about `cfg_s`
    va_arg(*vargs, size_t);
    struct cfg_s* restrict cfg = va_arg(*vargs, void*);
    cfg->clk.delta_t *= 0.9 * MAX(0.5, MIN(pow(E, -1.0 / (q + 1)), 2.0));
    return E <= 1.0;
}

