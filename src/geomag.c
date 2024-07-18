#include "geomag.h"
#include "quat.h"
#include "config.h"
#include "util.h"
#include "log.h"

const double Jm[(G_DEG+1)*(G_DEG+2)/2][2] = {
#if G_DEG >= 0
    {},  //   0   0
#if G_DEG >= 1
    {-29404.5E-9,       0.0E-9},  //   1   0
    { -1450.7E-9,    4652.9E-9},  //   1   1
#if G_DEG >= 2
    { -2500.0E-9,       0.0E-9},  //   2   0
    {  2982.0E-9,   -2991.6E-9},  //   2   1
    {  1676.8E-9,    -734.8E-9},  //   2   2
#if G_DEG >= 3
    {  1363.9E-9,       0.0E-9},  //   3   0
    { -2381.0E-9,     -82.2E-9},  //   3   1
    {  1236.2E-9,     241.8E-9},  //   3   2
    {   525.7E-9,    -542.9E-9},  //   3   3
#if G_DEG >= 4
    {   903.1E-9,       0.0E-9},  //   4   0
    {   809.4E-9,     282.0E-9},  //   4   1
    {    86.2E-9,    -158.4E-9},  //   4   2
    {  -309.4E-9,     199.8E-9},  //   4   3
    {    47.9E-9,    -350.1E-9}   //   4   4
#endif
#endif
#endif
#endif
#endif
};

const double Km[(G_DEG+1)*(G_DEG+2)/2] = {
#if G_DEG >= 0
    1.0,
#if G_DEG >= 1
    1.0, 1.0,
#if G_DEG >= 2
    1.0, sqrt(1.0 / 3.0), sqrt(1.0 / 12.0),
#if G_DEG >= 3
    1.0, sqrt(1.0 / 6.0), sqrt(1.0 / 60.0), sqrt(1.0 / 360.0),
#if G_DEG >= 4
    1.0, sqrt(1.0 / 10.0), sqrt(1.0 / 180.0), sqrt(1.0 / 2520.0), sqrt(1.0 / 20160.0)
#endif
#endif
#endif
#endif
#endif
};

bool geomag_eval(const vec_t r_bar, vec_t B_bar) {
    LOG_STATS("geomag_eval", 8, 21, 2);
    double r = vec_norm(r_bar);
    if (r < ABSTOL)
        return false;
    // convert position to spherical coordinates
    double a = sqrt(r_bar[0] * r_bar[0] + r_bar[1] * r_bar[1]) / r,
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
    // accumulate component fields
    double B_dot_r = 0.0, B_dot_th = 0.0, B_dot_ph = 0.0;
    for (size_t n = 1; n <= G_DEG; n++)
        for (size_t m = 0; m <= MIN(n, G_ORD); m++) {
            double foo = Km[n*(n+1)/2+m] * R[n] * P[n*(n+1)/2+m],
                   bar = Km[n*(n+1)/2+m] * R[n] * Q[n*(n+1)/2+m],
                   fizz = Jm[n*(n+1)/2+m][0] * C[m]
                        + Jm[n*(n+1)/2+m][1] * S[m],
                   buzz = Jm[n*(n+1)/2+m][1] * C[m] 
                        - Jm[n*(n+1)/2+m][0] * S[m];
            B_dot_r += (n + 1) * foo * fizz;
            B_dot_th += bar * fizz;
            B_dot_ph += m * ((a > ABSTOL) ? foo : bar) * buzz;
        }
    B_dot_th *= - R[2];
    B_dot_ph /= - R[2] * ((a > ABSTOL) ? a : z);
    // convert field to cartesian coordinates
    B_bar[0] = x * (a * B_dot_r + z * B_dot_th) - y * B_dot_ph;
    B_bar[1] = y * (a * B_dot_r + z * B_dot_th) + x * B_dot_ph;
    B_bar[2] = z * B_dot_r - a * B_dot_th;
    return true;
}

bool geomag(
    size_t size __attribute__((unused)),
    const struct cfg_s* cfg __attribute__((unused)),
    const struct st_s* st,
    struct in_s* restrict in __attribute__((unused)),
    const struct out_s* out,
    struct em_s* restrict em
) {
    LOG_STATS("geomag", 0, 0, 0);
    // rotate position into ECEF frame
    quat_t q_i2f;
    vec_t r_bar, B_bar;
    gee_quat_i2f(st, q_i2f);
    vec_rot(st->sys.q, out->sys.c_bar, r_bar);
    vec_add(st->sys.r_bar, r_bar, r_bar);
    vec_irot(q_i2f, r_bar, r_bar);
    
    if (!geomag_eval(r_bar, B_bar))
        return false;
    
    // rotate field into ECI frame
    vec_t temp;
    vec_rot(q_i2f, B_bar, temp);
    vec_add(em->sys.B_bar, temp, em->sys.B_bar);
    return true;
}

