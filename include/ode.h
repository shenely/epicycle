#ifndef __ODE_H__
#define __ODE_H__

/* Ordinary differential equation library
 * --------------------------------------
 */

/* Internal libraries */
#include "st.h"

/* Built-in libraries */
#include <stdarg.h>
#include <stddef.h>

#define ODE_METHOD_NAME(name) _ode_##name
#define ODE_METHOD_PARAMS (\
    double x0, const st_t* y0,\
    double x1, st_t* restrict y1,\
    ode_fun_t fun,\
    ode_step_t step __attribute__((unused)),\
    size_t nargs, va_list* vargs\
)
#define FUNCTION_ODE_METHOD(name) bool ODE_METHOD_NAME(name) ODE_METHOD_PARAMS

typedef void (*ode_fun_t) (double, const st_t*, st_t* restrict, size_t, va_list*);
typedef bool (*ode_step_t) (const st_t*, const st_t*, const st_t*, int, size_t, va_list*);
typedef bool (*ode_meth_t) ODE_METHOD_PARAMS;

/* Ordinary differential equation methods
 * :param double x0:
 * :param st_t* y0: input state
 * :param double x1:
 * :param st_t* y1: output state
 * :param ode_fun_t fun: accumulator function
 * :param ode_step_t fun: step function
 * :param size_t nargs: number of arguments
 * :param va_list* vargs: variadic arguments
 * :returns bool: repeat step
 */
FUNCTION_ODE_METHOD(euler);  // Euler method
FUNCTION_ODE_METHOD(verlet); // Verlet integration
FUNCTION_ODE_METHOD(rk4);    // Runge-Kutta (4th-order) method
FUNCTION_ODE_METHOD(heuler); // Heun-Euler method
FUNCTION_ODE_METHOD(dopri);  // Dormand-Prince method
FUNCTION_ODE_METHOD(beuler); // backward Euler method
FUNCTION_ODE_METHOD(midp);   // Midpointm method
FUNCTION_ODE_METHOD(vgl4);   // Gauss-Legendre (4th-order) method
FUNCTION_ODE_METHOD(vgl6);   // Gauss-Legendre (6th-order) method

/* Solve initial value problem
 * :param double x0:
 * :param st_t* y0: input state
 * :param double x1:
 * :param st_t* y1: output state
 * :param ode_meth_t meth: integration function
 * :param ode_fun_t fun: accumulator function
 * :param ode_step_t fun: step function
 * :param size_t nargs: number of arguments
 */
bool solve_ivp(double, const st_t*, double, st_t* restrict, ode_meth_t, ode_fun_t, ode_step_t, size_t, ...);

#endif  // __ODE_H__

