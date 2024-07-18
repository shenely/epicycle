#ifndef __FORCE_MODEL_H__
#define __FORCE_MODEL_H__
/* Force model library
 * -------------------
 */

/* External libraries */
#include <stddef.h>
#include <stdarg.h>

/* Internal libraries */
#include "data_model.h"
#include "ode.h"
#include "gmat.h"
#include "config.h"

/* Data types */
struct force_model_s {
    size_t size;
    ode_fun_t accum_fun;
    ode_step_t step_fun;
    union tol_u abstol;
    union tol_u reltol;
    bool (*fun_lst[16])(
        size_t,
        const struct cfg_s*,
        const struct st_s*,
        struct in_s* restrict,
        const struct out_s*,
        struct em_s* restrict
    );
};

/* Interpolate state
 * :param size_t size:
 * :param st_t* prev: previous state structure
 * :param st_t* next: next state structure
 * :param st_t* curr: current state structure
 */
void interp_st(
    size_t,
    const struct st_s*,
    const struct st_s*,
    struct st_s* restrict
);

/* Interpolate object list
 * :param size_t size:
 * :param st_t* prev: previous state structure
 * :param st_t* next: next state structure
 * :param st_t* curr: current state structure
 */
void interp_obj_lst(
    size_t,
    const struct st_s*,
    const struct st_s*,
    struct st_s* restrict
);

/* Solve state structure derivatives
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* prev: previous state structure
 * :param st_t* next: next state structure
 * :param in_t* in: input structure
 */
void solve_st_dot(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct st_s* restrict,
    const struct in_s*
);

/* Solve state structure differences
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* prev: previous state structure
 * :param st_t* next: next state structure
 * :param out_t* out: output structure
 */
void solve_st_delta(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct st_s* restrict,
    struct out_s* restrict
);

/* Solve change structure
 * :param size_t size:
 * :param ch_t* ch: change structure
 * :param st_t* prev: previous state structure
 * :param st_t* next: current state structure
 * :param in_t* in: input structure
 * :param em_t* em: electromagnetic structure
 */
bool solve_ch(
    size_t,
    struct ch_s* restrict,
    const struct st_s*,
    struct st_s* restrict,
    struct in_s* restrict,
    struct em_s* restrict
);

/* Solve input structure
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 */
void solve_in(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct in_s* restrict,
    const struct out_s*
);

/* Solve output structure
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param out_t* out: output structure
 */
void solve_out(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct out_s* restrict
);

/* Solve electromagnetic structure
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param em_t* out: electromagnetic structure
 */
void solve_em(
    size_t,
    const struct cfg_s*,
    struct em_s* restrict
);

/* Lorentz force
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 * :param em_t* out: electromagnetic structure
 * :returns bool:
 */
bool em(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct in_s* restrict,
    const struct out_s*,
    struct em_s* restrict
);

/* Apply force model
 * :param double x:
 * :param gvec_t y: input position vector
 * :param gvec_t f: output force vector
 * :param size_t nargs: number of arguments
 * :param va_list* vargs: variadic arguments
 */
void apply_force_model(
    double, const gvec_t, gvec_t,
    size_t, va_list*
);

/* Adjust time step
 * :param gvec_t y0: (first) input vector
 * :param gvec_t y1: (second) input vector
 * :param gvec_t y2: (third) input vector
 * :param int q: integration order
 * :param size_t nargs: number of arguments
 * :param va_list* vargs: variadic arguments
 */
bool adjust_time_step(
    const gvec_t, const gvec_t, const gvec_t,
    const gvec_t, const gvec_t,
    int, size_t, va_list*
);

#endif  // __FORCE_MODEL_H__

