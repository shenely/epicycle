#ifndef __STDATM_H__
#define __STDATM_H__
/* Standard atmosphere library
 * ---------------------------
 */

/* Internal libraries */
#include <stdbool.h>

/* Internal libraries */
#include "data_model.h"
#include "poly.h"

/* Constants */
#define A_RSTAR 8.31432
#define A_G0 9.80665
#define A_M0 28.9644e-3
#define A_T0 288.15
#define A_P0 101325.0
#define A_G7 9.5466
#define A_M7 28.95e-3
#define A_T7 186.87

/* Data types */
struct atm_s {  // atmosphere condiion structure
    double th;  // temperature
    double p;   // pressure
    double rho; // density
};

struct uasa20_s { // unofficial Australian standard atmosphere
    double z;
    double q;
    union {
        struct {
            size_t n;
            double a[4];
        };
        struct poly_s p;
    };
};

/* Initialize standard atmosphere */
void stdatm_init();

/* Evalute standard atmosphere force model */
bool stdatm_eval(double, struct atm_s* restrict);

/* Standard atmosphere force model
 * :param size_t size:
 * :param cfg_t* cfg: configuration structure
 * :param st_t* st: state structure
 * :param in_t* in: input structure
 * :param out_t* out: output structure
 * :param em_t* em: electromagnetic structure
 * :returns bool:
 */
bool stdatm(
    size_t,
    const struct cfg_s*,
    const struct st_s*,
    struct in_s* restrict,
    const struct out_s*,
    struct em_s* restrict
);

#endif  // __STDATM_H__

