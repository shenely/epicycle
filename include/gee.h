#ifndef __GEE_H__
#define __GEE_H__
/* Earth gravity library
 * ---------------------
 */

/* Internal libraries */
#include <stddef.h>
#include <stdbool.h>

/* Internal libraries */
#include "data_model.h"
#include "quat.h"

/* Constants */
#define G_MU 3986004.415E+8
#define G_RMAX 6378136.3
#define G_RMIN 6356766.0
#define G_INVF 298.4579673659263
#define G_DEG 4
#define G_ORD 4

/* ECI to ECEF quaternion
 * :param st_t* st: state structure
 * :param quat_t* curr: output quaternion
 */
void gee_quat_i2f(const struct st_s*, quat_t);

/* ECEF to geodetic
 * :param vec_t* st: input vector
 * :param double* lat: geodetic latitude
 * :param double* lon: longitude
 * :param double* alt: geodetic altitude
 * :returns bool: solution converged
 */
bool gee_f2d(const vec_t, double*, double*, double*);

/* Inverse square law
 * :param vec_t* st: input vector
 * :param double* r__2: square of distance
 * :param double* g: force of gravity
 * :returns bool:
 */
bool inv_sq_law(const vec_t, double*, double*);

/* Spherical harmonics */
void sph_harm(
    double, double, double, double, double,
    double*, double*, double*, double*, double*
);

/* Gravity force model
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 * :param em_t* em: electromagnetic structure
 * :returns bool:
 */
bool gee(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct in_s* restrict,
    const struct out_s*,
    struct em_s* restrict
);

/* Gravity force model (fast)
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 * :param em_t* em: electromagnetic structure
 * :returns bool:
 */
bool gee_fast(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct in_s* restrict,
    const struct out_s*,
    struct em_s* restrict
);

/* Evalute geopotential and geomagnetic force model */
bool geoall_eval(double, const vec_t, vec_t, vec_t);

/* Geopotential and geomagnetic force model
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 * :param em_t* em: electromagnetic structure
 * :returns bool:
 */
bool geoall(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct in_s* restrict,
    const struct out_s*,
    struct em_s* restrict
);

#endif  // __GEE_H__

