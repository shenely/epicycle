#include <math.h>
#include "stdatm.h"
#include "gee.h"
#include "util.h"
#include "log.h"

static struct uasa20_s __uasa20_p[16] = {
    {.z=  86e3, .q=3.7338e-1, .p={.deg=3}},
    {.z= 100e3, .q=3.2011e-2, .p={.deg=3}},
    {.z= 115e3, .q=4.0096e-3, .p={.deg=3}},
    {.z= 130e3, .q=1.2505e-3, .p={.deg=3}},
    {.z= 150e3, .q=4.5422e-4, .p={.deg=3}},
    {.z= 175e3, .q=1.7936e-4, .p={.deg=3}},
    {.z= 200e3, .q=8.4736e-5, .p={.deg=3}},
    {.z= 250e3, .q=2.4767e-5, .p={.deg=3}},
    {.z= 300e3, .q=8.7704e-6, .p={.deg=3}},
    {.z= 400e3, .q=1.4518e-6, .p={.deg=3}},
    {.z= 500e3, .q=3.0236e-7, .p={.deg=3}},
    {.z= 600e3, .q=8.2130e-8, .p={.deg=3}},
    {.z= 700e3, .q=3.1908e-8, .p={.deg=3}},
    {.z= 800e3, .q=1.7036e-8, .p={.deg=3}},
    {.z= 900e3, .q=1.0873e-8, .p={.deg=3}},
    {.z=1000e3, .q=7.5138e-9}
};

static struct uasa20_s __uasa20_rho[16] = {
    {.z=  86e3, .q=6.958e-06, .p={.deg=3}},
    {.z= 100e3, .q=5.604e-07, .p={.deg=3}},
    {.z= 115e3, .q=4.289e-08, .p={.deg=3}},
    {.z= 130e3, .q=8.152e-09, .p={.deg=3}},
    {.z= 150e3, .q=2.076e-09, .p={.deg=3}},
    {.z= 175e3, .q=6.339e-10, .p={.deg=3}},
    {.z= 200e3, .q=2.541e-10, .p={.deg=3}},
    {.z= 250e3, .q=6.073e-11, .p={.deg=3}},
    {.z= 300e3, .q=1.916e-11, .p={.deg=3}},
    {.z= 400e3, .q=2.803e-12, .p={.deg=3}},
    {.z= 500e3, .q=5.215e-13, .p={.deg=3}},
    {.z= 600e3, .q=1.137e-13, .p={.deg=3}},
    {.z= 700e3, .q=3.070e-14, .p={.deg=3}},
    {.z= 800e3, .q=1.136e-14, .p={.deg=3}},
    {.z= 900e3, .q=5.759e-15, .p={.deg=3}},
    {.z=1000e3, .q=3.561e-15}
};

static void uasa20_init(struct uasa20_s* restrict pt) {
    double h[16], a[16], b[16], c[16], d[16], x[16], y[16], k[16];
    for (size_t i = 0; i < 16; i++) {
        b[i] = 2.0;
        x[i] = pt[i].z;
        y[i] = log(pt[i].q);
    }
    for (size_t i = 1; i < 16; i++)
        h[i] = pt[i].z - pt[i-1].z;
    c[0] = 1.0;
    d[0] = (6.0 / h[1]) * ((y[1] - y[0]) / h[1] + A_G7 * (A_M7 / A_RSTAR) / A_T7);
    for (size_t i = 1; i < 15; i++) {
        a[i] = h[i] / (h[i] + h[i+1]);
        c[i] = h[i+1] / (h[i] + h[i+1]);
        d[i] = 6.0 * ((y[i+1] - y[i]) / h[i+1] - (y[i] - y[i-1]) / h[i]) / (x[i+1] - x[i-1]);
    }
    a[15] = 1.0;
    d[15] = - (6.0 / h[15]) * (y[15] - y[14]) / h[15];
    for (size_t i = 1; i < 16; i++) {
        double w = a[i] / b[i-1];
        b[i] -= w * c[i-1];
        d[i] -= w * d[i-1];
    }
    k[15] = d[15] / b[15];
    for (int i = 14; i > -1; i--) {
        k[i] = (d[i] - c[i] * k[i+1]) / b[i];
        pt[i].p.coeff[0] = - (
            (k[i+1] * x[i] * x[i] * x[i] - k[i] * x[i+1] * x[i+1] * x[i+1])
            - h[i+1] * h[i+1] * (k[i+1] * x[i] - k[i] * x[i+1])
            + 6.0 * (y[i+1] * x[i] - y[i] * x[i+1])
        ) / (6.0 * h[i+1]);
        pt[i].p.coeff[1] = (
            3.0 * (k[i+1] * x[i] * x[i] - k[i] * x[i+1] * x[i+1])
            - h[i+1] * h[i+1] * (k[i+1] - k[i])
            + 6.0 * (y[i+1] - y[i])
        ) / (6.0 * h[i+1]);
        pt[i].p.coeff[2] = - 3.0 * (k[i+1] * x[i] - k[i] * x[i+1]) / (6.0 * h[i+1]);
        pt[i].p.coeff[3] = (k[i+1] - k[i]) / (6.0 * h[i+1]);
    }
}

void stdatm_init() {
    uasa20_init(__uasa20_p);
    uasa20_init(__uasa20_rho);
}

bool stdatm_eval(double z, struct atm_s* restrict atm) {
    if (z < 86e3) {
        LOG_STATS("stdatm_eval", 15, 30, 7);
        static double R = A_RSTAR / A_M0, C = A_G0 * A_M0 / A_RSTAR,
               hmax = G_RMIN * 86e3 / (G_RMIN + 86e3);
        double h = G_RMIN * z / (G_RMIN + z), delta_th;
        atm->th = A_T0;
        atm->p = A_P0;
        // from 0km to 11km
        if (z > 0) {
            delta_th = -6.5e-3 *  MIN(    h, 11e3);
            atm->p *= pow((atm->th / (atm->th + delta_th)), C / -6.5e-3);
            atm->th += delta_th;
        }
        // from 11km to 20km
        if (z > 11e3) {
            atm->p *= exp(- C * (MIN(MAX(h, 11e3), 20e3) - 11e3) / atm->th);
            // from 20km to 32km
            delta_th = +1.0e-3 * (MIN(MAX(h, 20e3), 32e3) - 20e3);
            atm->p *= pow((atm->th / (atm->th + delta_th)), C / +1.0e-3);
            atm->th += delta_th;
        }
        // from 32km to 47km
        if (z > 32e3) {
            delta_th = +2.8e-3 * (MIN(MAX(h, 32e3), 47e3) - 32e3);
            atm->p *= pow((atm->th / (atm->th + delta_th)), C / +2.8e-3);
            atm->th += delta_th;
        }
        // from 47km to 51km
        if (z > 47e3) {
            atm->p *= exp(- C * (MIN(MAX(h, 47e3), 51e3) - 47e3) / atm->th);
        }
        // from 51km to 71km
        if (z > 51e3) {
            delta_th = -2.8e-3 * (MIN(MAX(h, 51e3), 71e3) - 51e3);
            atm->p *= pow((atm->th / (atm->th + delta_th)), C / -2.8e-3);
            atm->th += delta_th;
        }
        // from 71km to 86km
        if (z > 71e3) {
            delta_th = -2.0e-3 * (MIN(MAX(h, 71e3), hmax) - 71e3);
            atm->p *= pow((atm->th / (atm->th + delta_th)), C / -2.0e-3);
            atm->th += delta_th;
        }
        // p = rho * (R* / M0) * th
        atm->rho = atm->p / (R * atm->th);
        // if z > 80e3:
        //     th *= M(z) / M0
    } else {
        if (z < 91e3) {
            atm->th = 186.8673;
        } else if (z < 110e3) {
            LOG_STATS("stdatm_eval", 3, 2, 2);
            atm->th = 263.1905 - 76.3232 * sqrt(1 - pow((z - 91e3) / -19.9429e3, 2.0));
        } else if (z < 120e3) {
            LOG_STATS("stdatm_eval", 2, 1, 0);
            atm->th = 240.0 + 12e-3 * (z - 110e3);
        } else if (z <= 1000e3) {
            LOG_STATS("stdatm_eval", 4, 4, 1);
            atm->th = 1000.0 - 640.0 * exp(-0.01875e-3 * (z - 120e3) * (G_RMIN + 120e3) / (G_RMIN + z));
        } else {
            return false;
        }
            LOG_STATS("stdatm_eval", 0, 0, 2);
        for (size_t i = 0; i < 15; i++) {
            if ((__uasa20_p[i].z <= z) && (z <= __uasa20_p[i+1].z)) {
                atm->p = exp(poly_eval(&__uasa20_p[i].p, z));
                atm->rho = exp(poly_eval(&__uasa20_rho[i].p, z));
                break;
            }
        }
    }
    return true;
}

bool stdatm(
    size_t size,
    const struct cfg_s* cfg,
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out __attribute__((unused)),
    struct em_s* restrict em __attribute__((unused))
) {
    LOG_STATS("stdatm", 0, 0, 0);
    double v = vec_norm(&st->sys.v_bar), z, A, F;
    vec_t v_bar, foo, bar;
    struct atm_s atm;
    gee_f2d(&st->sys.r_bar, NULL, NULL, &z);
    if (!stdatm_eval(z, &atm))
        return true;
    vec_irot(&st->sys.q, &st->sys.v_bar, &v_bar);
    vec_zero(&foo);
    for (size_t idx = 0; idx < size; idx++) {
        LOG_STATS("stdatm", 3, 12, 0);
        vec_t temp;
        // x-axis
        vec_rot(&cfg->obj_lst[idx].q, &i_hat, &temp);
        A = cfg->obj_lst[idx].bbox[1] * cfg->obj_lst[idx].bbox[2];
        F = atm.rho * v * vec_dot(&v_bar, &temp) * A;
        vec_muls(&temp, - F, &temp);
        vec_add(&foo, &temp, &foo);
        vec_cross(&cfg->obj_lst[idx].r_bar, &temp, &bar);
        vec_add(&in->sys.M_bar, &bar, &in->sys.M_bar);
        // y-axis
        vec_rot(&cfg->obj_lst[idx].q, &j_hat, &temp);
        A = cfg->obj_lst[idx].bbox[2] * cfg->obj_lst[idx].bbox[0];
        F = atm.rho * v * vec_dot(&v_bar, &temp) * A;
        vec_muls(&temp, - F, &temp);
        vec_add(&foo, &temp, &foo);
        vec_cross(&cfg->obj_lst[idx].r_bar, &temp, &bar);
        vec_add(&in->sys.M_bar, &bar, &in->sys.M_bar);
        // z-axis
        vec_rot(&cfg->obj_lst[idx].q, &k_hat, &temp);
        A = cfg->obj_lst[idx].bbox[0] * cfg->obj_lst[idx].bbox[1];
        F = atm.rho * v * vec_dot(&v_bar, &temp) * A;
        vec_muls(&temp, - F, &temp);
        vec_add(&foo, &temp, &foo);
        vec_cross(&cfg->obj_lst[idx].r_bar, &temp, &bar);
        vec_add(&in->sys.M_bar, &bar, &in->sys.M_bar);
    }
    vec_rot(&st->sys.q, &foo, &bar);
    vec_add(&in->sys.F_bar, &bar, &in->sys.F_bar);
    return true;
}

