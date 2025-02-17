#ifndef __GEOPOT_H__
#define __GEOPOT_H__

/* Geopotential library
 * --------------------
 */

/* Internal libraries */
#include "gee.h"

/* Built-in libraries */
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

extern const double Jg[(G_DEG+1)*(G_DEG+2)/2][2];
extern const double Kg[(G_DEG+1)*(G_DEG+2)/2];

/* Evalute geopotential force model */
bool geopot_eval(double, const vec_t, vec_t);

/* Geopotential force model
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 * :param em_t* em: electromagnetic structure
 * :returns bool:
 */
bool geopot(size_t, const struct cfg_s*, const struct st_s*, struct in_s* restrict, const struct out_s*, struct em_s* restrict);

#endif  // __GEOPOT_H__

