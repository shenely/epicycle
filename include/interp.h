#ifndef __INTERP_H__
#define __INTERP_H__

/* Interpolation library
 * ---------------------
 */

/* Internal libraries */
#include "vec.h"
#include "quat.h"

/* Initialize interpolatin */
void interp_init();

/* Linear interpolation
 * :param double x0:
 * :param double y0: (first) input scalar
 * :param double x1:
 * :param double y1: (second) input scalar
 * :param double x:
 * :param double y: output scalar
 */
double interp_lerp(double, double, double, double, double);

/* (Vector) linear interpolation
 * :param double x0: 
 * :param double v0_bar: (first) input vector
 * :param double x1:
 * :param double v1_bar: (second) input vector
 * :param double x:
 * :param double v_bar: output vector
 */
void interp_vlerp(double, const vec_t, double, const vec_t, double, vec_t);

/* (Spherical) linear interpolation
 * :param double x0:
 * :param double q0: (first) input quaternion
 * :param double x1:
 * :param double q1: (second) input quaternion
 * :param double x:
 * :param double q: output quaternion
 */
void interp_slerp(double, const quat_t, double, const quat_t, double, quat_t);

/* (Hermite) cubic spline interpolation
 * :param double x0:
 * :param double p0_bar: (first) input position vector
 * :param double m0_bar: (first) input velocity vector
 * :param double x1:
 * :param double p1_bar: (second) input postion vector
 * :param double m1_bar: (second) input velocity vector
 * :param double x:
 * :param double p_bar: output postion vector
 * :param double m_bar: output velocity vector
 */
void interp_cint(double, const vec_t, const vec_t, double, const vec_t, const vec_t, double, vec_t, vec_t);

/* (Spherical) cubic spline interpolation
 * :param double x0:
 * :param double q0: (first) input quaternion
 * :param double om0_bar: (first) input angular velocity
 * :param double x1:
 * :param double q1: (second) input quaternion
 * :param double om1_bar: (second) input angular velocity
 * :param double x:
 * :param double q: output quaternion
 * :param double om_bar: output angular velocity
 */
void interp_squad(double, const quat_t, const vec_t, double, const quat_t, const vec_t, double, quat_t, vec_t);

#endif  // __INTERP_H__

