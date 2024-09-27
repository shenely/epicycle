#include "geopot.h"
#include "vec.h"
#include "quat.h"
#include "config.h"
#include "util.h"
#include "log.h"

const double Jg[(G_DEG+1)*(G_DEG+2)/2][2] = {
#if G_DEG >= 0
    { 0.000000000000E+00,  0.000000000000E+00},  //   0   0
#if G_DEG >= 1
    { 0.000000000000E+00,  0.000000000000E+00},  //   1   0
    { 0.000000000000E+00,  0.000000000000E+00},  //   1   1
#if G_DEG >= 2
    {-0.484165371736E-03,  0.000000000000E+00},  //   2   0
    {-0.186987635955E-09,  0.119528012031E-08},  //   2   1
    { 0.243914352398E-05, -0.140016683654E-05},  //   2   2
#if G_DEG >= 3
    { 0.957254173792E-06,  0.000000000000E+00},  //   3   0
    { 0.202998882184E-05,  0.248513158716E-06},  //   3   1
    { 0.904627768605E-06, -0.619025944205E-06},  //   3   2
    { 0.721072657057E-06,  0.141435626958E-05},  //   3   3
#if G_DEG >= 4
    { 0.539873863789E-06,  0.000000000000E+00},  //   4   0
    {-0.536321616971E-06, -0.473440265853E-06},  //   4   1
    { 0.350694105785E-06,  0.662671572540E-06},  //   4   2
    { 0.990771803829E-06, -0.200928369177E-06},  //   4   3
    {-0.188560802735E-06,  0.308853169333E-06}   //   4   4
#endif
#endif
#endif
#endif
#endif
};

const double Kg[(G_DEG+1)*(G_DEG+2)/2] = {
#if G_DEG >= 0
    1.0,
#if G_DEG >= 1
    1.7320508075688772, 1.7320508075688772,
#if G_DEG >= 2
    2.23606797749979, 1.2909944487358056, 0.6454972243679028,
#if G_DEG >= 3
    2.6457513110645907, 1.0801234497346435, 0.31622776601683794, 0.13944333775567927,
#if G_DEG >= 4
    3.0, 0.9486832980505138, 0.22360679774997896, 0.05976143046671968, 0.021128856368212913
#endif
#endif
#endif
#endif
#endif
};

bool geopot_eval(double m, const vec_t r_bar, vec_t F_bar) {
    LOG_STATS("geopot_eval", 9, 23, 2);
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
    double F_dot_r = 0.0, F_dot_th = 0.0, F_dot_ph = 0.0;
    for (size_t n = 2; n <= G_DEG; n++)
        for (size_t m = 0; m <= MIN(n, G_ORD); m++) {
            LOG_STATS("geopot_eval", 5, 13, 0);
            double foo = Kg[n*(n+1)/2+m] * R[n] * P[n*(n+1)/2+m],
                   bar = Kg[n*(n+1)/2+m] * R[n] * Q[n*(n+1)/2+m],
                   fizz = Jg[n*(n+1)/2+m][0] * C[m]
                        + Jg[n*(n+1)/2+m][1] * S[m],
                   buzz = Jg[n*(n+1)/2+m][1] * C[m] 
                        - Jg[n*(n+1)/2+m][0] * S[m];
            F_dot_r += (n + 1) * foo * fizz;
            F_dot_th += bar * fizz;
            F_dot_ph += m * ((a > ABSTOL) ? foo : bar) * buzz;
        }
    F_dot_r  *= - g;
    F_dot_th *= - g;
    F_dot_ph *= - g / ((a > ABSTOL) ? a : z);
    // convert force to cartesian coordinates
    F_bar[0] = x * (a * F_dot_r + z * F_dot_th) - y * F_dot_ph;
    F_bar[1] = y * (a * F_dot_r + z * F_dot_th) + x * F_dot_ph;
    F_bar[2] = z * F_dot_r - a * F_dot_th;
    return true;
}

bool geopot(
    size_t size __attribute__((unused)),
    const struct cfg_s* cfg __attribute__((unused)),
    const struct st_s* st,
    struct in_s* restrict in,
    const struct out_s* out,
    struct em_s* restrict em __attribute__((unused))
) {
    LOG_STATS("geopot", 0, 0, 0);
    // rotate position into ECEF frame
    quat_t q_i2f;
    vec_t r_bar, F_bar, M_bar;
    gee_quat_i2f(st, q_i2f);
    vec_rot(st->sys.q, out->sys.c_bar, r_bar);
    vec_add(st->sys.r_bar, r_bar, r_bar);
    vec_irot(q_i2f, r_bar, r_bar);
    
    if (!geopot_eval(out->sys.m, r_bar, F_bar))
        return false;
    
    // rotate force into ECI frame
    vec_rot(q_i2f, F_bar, F_bar);
    vec_add(in->sys.F_bar, F_bar, in->sys.F_bar);
    vec_irot(st->sys.q, F_bar, F_bar);
    vec_cross(out->sys.c_bar, F_bar, M_bar);
    vec_add(in->sys.M_bar, M_bar, in->sys.M_bar);
    return true;
}

