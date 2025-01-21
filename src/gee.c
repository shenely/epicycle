#include <math.h>
#include "gee.h"
#include "geopot.h"
#include "geomag.h"
#include "vec.h"
#include "mat.h"
#include "quat.h"
#include "dmat.h"
#include "poly.h"
#include "config.h"
#include "util.h"
#include "log.h"

static const struct poly_s __th_G0 = {.deg=3, .coeff={
    100.4606184,
    36000.77004,
    0.000387933, 
    -2.583e-8
}};

void gee_quat_i2f(
    const struct st_s* st,
    quat_t q
) {
    LOG_STATS("gee_quat_i2f", 6, 7, 0);
    double JD = st->clk.t / 86400 + 2440587.5,
           J0 = floor(JD + 0.5) - 0.5,
           UT = 24 * (JD - J0),
           T0 = (J0 - 2451545) / 36525,
           th_G = poly_eval(&__th_G0, T0) + (360.98564724 / 24.0) * UT;
    vec_t foo = {0.0, 0.0, 0.5 * th_G * M_PI_180};
    vec_exp(foo, q);
}

bool gee_f2d(const vec_t r_bar, double* lat, double* lon, double* alt) {
    LOG_STATS("gee_f2d", 5, 14, 5);
    double p__2 = r_bar[0] * r_bar[0] + r_bar[1] * r_bar[1],
           p = sqrt(p__2),
           z = r_bar[2],
           z__2 = z * z,
           e__2 = (2.0 - 1.0 / G_INVF) / G_INVF,
           ae__2 = G_RMAX * e__2,
           inv_k0 = pow(1 - 1.0 / G_INVF, 2.0),
           k = 1.0 / inv_k0;
    bool done = false;
    for (size_t n = 0; n < 50; n++) {
        LOG_STATS("gee_f2d", 5, 10, 1);
        double c = pow(p__2 + inv_k0 * z__2 * k * k, 1.5) / ae__2;
        double delta_k = (c + inv_k0 * z__2 * k * k * k) / (c - p__2) - k;
        k += delta_k;
        done = fabs(delta_k) < ABSTOL + RELTOL * fabs(k);
        if (done)
            break;
    }
    if (lat != NULL) *lat = atan(k * z / p);
    if (lon != NULL) *lon = atan2(r_bar[1], r_bar[0]);
    if (alt != NULL) *alt = (1.0 / k - inv_k0) * sqrt(p__2 + z__2 * k * k) / e__2;
    return done;
}

bool inv_sq_law(const vec_t r_bar, double* r__2, double* g) {
    LOG_STATS("inv_sq_law", 0, 3, 0);
    double r = vec_norm(r_bar);
    if (r < ABSTOL)
        return false;
    *r__2 = r * r;
    if (g != NULL) *g = G_MU / (*r__2 * r);
    return true;
}

void sph_harm(
    double r, double a, double x, double y, double z,
    double* R, double* P, double* Q, double* C, double* S 
) {
    LOG_STATS("sph_harm", 0, 0, 0);
    R[0] = 1.0; R[1] = G_RMAX / r;
    P[0] = 1.0; P[1] = z;
    Q[0] = 0.0; Q[1] = 1.0;
    C[0] = 1.0; C[1] = x; 
    S[0] = 0.0; S[1] = y;
    for (size_t n = 2; n <= MAX(G_DEG, 2); n++)
        R[n] = R[1] * R[n-1];
    for (size_t n = 1; n <= G_DEG; n++) {
        if (n > 1) {
            P[n*(n+1)/2] = (  (2 * n - 1) * z * P[(n-1)*n/2]
                            -     (n - 1) * P[(n-2)*(n-1)/2]) / n;
            Q[n*(n+1)/2] = (n * P[(n-1)*n/2] + z * Q[(n-1)*n/2]) * a;
        }
        for (size_t m = 1; m <= MIN(n, G_ORD); m++) {
            if (a > ABSTOL) {
                P[n*(n+1)/2+m] = (  (n - m + 1) * z * P[n*(n+1)/2+m-1]
                                  - (n + m - 1) *     P[(n-1)*n/2+m-1]) / a;
                Q[n*(n+1)/2+m] = (  (n + m)      * P[(n-1)*n/2+m]
                                  -  n      * z  * P[n*(n+1)/2+m]) / a;
            } else {
                P[n*(n+1)/2+m] = 0.0;
                Q[n*(n+1)/2+m] = 0.0;
            }
        }
    }
    for (size_t m = 2; m <= G_ORD; m++) {
        C[m] = 2.0 * x * C[m-1] - C[m-2];
        S[m] = 2.0 * x * S[m-1] - S[m-2];
    }
}

bool gee(
    size_t size,
    const struct cfg_s* cfg,
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out __attribute__((unused)),
    struct em_s* restrict em __attribute__((unused))
) {
    LOG_STATS("gee", 2 * size, 2 * size, 0);
    vec_t c_bar, F_bar, temp;
    vec_zero(temp);
    vec_irot(st->sys.q, st->sys.r_bar, c_bar);
    for (size_t idx = 0; idx < size; idx++) {
        vec_t r_bar, foo, bar;
        vec_add(c_bar, cfg->obj_lst[idx].r_bar, r_bar);
        // gravity
        double r__2, g;
        if (!inv_sq_law(r_bar, &r__2, &g))
            return false;
        // point mass
        vec_muls(r_bar, - st->obj_lst[idx].m * g, foo);
        vec_add(temp, foo, temp);
        vec_cross(cfg->obj_lst[idx].r_bar, foo, bar);
        vec_add(in->sys.M_bar, bar, in->sys.M_bar);
        // rigid body
        dmat_mulv(st->obj_lst[idx].I_cm, r_bar, foo);
        vec_cross(r_bar, foo, bar);
        vec_muls(bar, 3.0 * g / r__2, bar);
        vec_add(in->sys.M_bar, bar, in->sys.M_bar);
    }
    vec_rot(st->sys.q, temp, F_bar);
    vec_add(in->sys.F_bar, F_bar, in->sys.F_bar);
    return true;
}

bool gee_fast(
    size_t size __attribute__((unused)),
    const struct cfg_s* cfg __attribute__((unused)),
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out,
    struct em_s* restrict em __attribute__((unused))
) {
    LOG_STATS("gee_fast", 2, 2, 0);
    vec_t r_bar, foo, bar;
    vec_irot(st->sys.q, st->sys.r_bar, r_bar);
    vec_add(r_bar, out->sys.c_bar, r_bar);
    // gravity
    double r__2, g;
    if (!inv_sq_law(r_bar, &r__2, &g))
        return false;
    // point mass
    vec_muls(r_bar, - out->sys.m * g, foo);
    vec_cross(out->sys.c_bar, foo, bar);
    vec_add(in->sys.M_bar, bar, in->sys.M_bar);
    vec_rot(st->sys.q, foo, bar);
    vec_add(in->sys.F_bar, bar, in->sys.F_bar);
    // rigid body
    mat_mulv(out->sys.I_cm, r_bar, foo);
    vec_cross(r_bar, foo, bar);
    vec_muls(bar, 3.0 * g / r__2, bar);
    vec_add(in->sys.M_bar, bar, in->sys.M_bar);
    return true;
}

bool geoall_eval(
    double m, const vec_t r_bar,
    vec_t F_bar, vec_t B_bar
) {
    LOG_STATS("geoall_eval", 17, 37, 2);
    double r__2;
    if (!inv_sq_law(r_bar, &r__2, NULL))
        return false;
    // convert position to spherical coordinates
    double g = G_MU * m / r__2,
           r = sqrt(r__2),
           a = sqrt(r_bar[0] * r_bar[0] + r_bar[1] * r_bar[1]) / r,
           x, y, z = r_bar[2] / r;
    if (a > ABSTOL) {
        x = r_bar[0] / r / a;
        y = r_bar[1] / r / a;
    } else {
        x = r_bar[0] / r * a;
        y = r_bar[1] / r * a;
    }
    // calculate spherical harmonics
    double R[MAX(G_DEG,2)+1], C[G_ORD+1], S[G_ORD+1],
           P[(G_DEG+1)*(G_DEG+2)/2],
           Q[(G_DEG+1)*(G_DEG+2)/2];
    sph_harm(r, a, x, y ,z, R, P, Q, C, S);
    // accumulate component forces
    double F_dot_r = 0.0, F_dot_th = 0.0, F_dot_ph = 0.0,
           B_dot_r = 0.0, B_dot_th = 0.0, B_dot_ph = 0.0;
    for (size_t n = 1; n <= G_DEG; n++)
        for (size_t m = 0; m <= MIN(n, G_ORD); m++) {
            LOG_STATS("geoall_eval", 8, 22, 0);
            double foo = Kg[n*(n+1)/2+m] * R[n] * P[n*(n+1)/2+m],
                   bar = Kg[n*(n+1)/2+m] * R[n] * Q[n*(n+1)/2+m],
                   fizz = Jg[n*(n+1)/2+m][0] * C[m]
                        + Jg[n*(n+1)/2+m][1] * S[m],
                   buzz = Jg[n*(n+1)/2+m][1] * C[m] 
                        - Jg[n*(n+1)/2+m][0] * S[m];
            F_dot_r += (n + 1) * foo * fizz;
            F_dot_th += bar * fizz;
            F_dot_ph += m * ((a > ABSTOL) ? foo : bar) * buzz;
            foo = Km[n*(n+1)/2+m] * R[n] * P[n*(n+1)/2+m];
            bar = Km[n*(n+1)/2+m] * R[n] * Q[n*(n+1)/2+m];
            B_dot_r += (n + 1) * foo * fizz;
            B_dot_th += bar * fizz;
            B_dot_ph += m * ((a > ABSTOL) ? foo : bar) * buzz;
        }
    F_dot_r  *= - g;
    F_dot_th *= - g;
    F_dot_ph *= - g / ((a > ABSTOL) ? a : z);
    B_dot_th *= - R[2];
    B_dot_ph /= - R[2] * ((a > ABSTOL) ? a : z);
    // convert force to cartesian coordinates
    F_bar[0] = x * (a * F_dot_r + z * F_dot_th) - y * F_dot_ph;
    F_bar[1] = y * (a * F_dot_r + z * F_dot_th) + x * F_dot_ph;
    F_bar[2] = z * F_dot_r - a * F_dot_th;
    B_bar[0] = x * (a * B_dot_r + z * B_dot_th) - y * B_dot_ph;
    B_bar[1] = y * (a * B_dot_r + z * B_dot_th) + x * B_dot_ph;
    B_bar[2] = z * B_dot_r - a * B_dot_th;
    return true;
}

bool geoall(
    size_t size __attribute__((unused)),
    const struct cfg_s* cfg __attribute__((unused)),
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out,
    struct em_s* restrict em
) {
    LOG_STATS("geoall", 0, 0, 0);
    double r__2;
    if (!inv_sq_law(st->sys.r_bar, &r__2, NULL))
        return false;
    // rotate position into ECEF frame
    quat_t q_i2f;
    vec_t r_bar, F_bar, M_bar, B_bar;
    gee_quat_i2f(st, q_i2f);
    vec_rot(st->sys.q, out->sys.c_bar, r_bar);
    vec_add(st->sys.r_bar, r_bar, r_bar);
    vec_irot(q_i2f, r_bar, r_bar);
    
    if (!geoall_eval(out->sys.m, r_bar, F_bar, B_bar))
        return false;
        
    // rotate force into ECI frame
    vec_rot(q_i2f, F_bar, F_bar);
    vec_add(in->sys.F_bar, F_bar, in->sys.F_bar);
    vec_irot(st->sys.q, F_bar, F_bar);
    vec_cross(out->sys.c_bar, F_bar, M_bar);
    vec_add(in->sys.M_bar, M_bar, in->sys.M_bar);
    vec_rot(q_i2f, B_bar, B_bar);
    vec_add(em->sys.B_bar, B_bar, em->sys.B_bar);
    return true;
}

