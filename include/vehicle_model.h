#ifndef __VEHICLE_MODEL_H__
#define __VEHICLE_MODEL_H__

/* Vehicle model library
 * ------------------
 */

/* Internal libraries */
#include "dmat.h"
#include "mat.h"
#include "quat.h"
#include "st.h"
#include "util.h"
#include "vec.h"

/* Built-in libraries */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined __XSMALL__
#define MAX_OBJ_COUNT 1
#elif defined __SMALL__
#define MAX_OBJ_COUNT 4
#elif defined __MEDIUM__
#define MAX_OBJ_COUNT 16
#elif defined __LARGE__
#define MAX_OBJ_COUNT 64
#elif defined __XLARGE__
#define MAX_OBJ_COUNT 256
#elif !defined MAX_OBJ_COUNT
#error "Maximum object count undefined"
#endif

/* Data types */
struct vehicle_model_s {
    size_t size;
    struct cfg_s {
        struct {
            double delta_t;
        } clk;
        struct {
            char sym[4];
        } sys;
        struct {
            char sym[4];
            dmat_t bbox;  // bounding box
            vec_t r_bar;  // position vector
            quat_t q;  // attitude quaternion
        } obj_lst[MAX_OBJ_COUNT];
    } cfg;
    struct st_s {
        struct {
            uint64_t n;
            double t;
        } clk;
        st_t sys;
        struct {
            double m;  // mass
            dmat_t I_cm;  // moment of inertia
            vec_t p_bar;  // momentum vector
            vec_t h_bar;  // angular momentum
        } obj_lst[MAX_OBJ_COUNT];
    } st;
    struct ch_s {
        struct {
            double t;
        } clk;
        struct {
            enum {E_NA, E_ST, E_IN, E_EM} T;
            union {
                struct {
                    double m;  // mass
                    vec_t p_bar;  // momentum vector
                    vec_t h_bar;  // angular momentum
                } st;
                struct {
                    double m_dot;  // mass flow rate
                    vec_t F_bar;  // force vector
                    vec_t M_bar;  // torque vector
                } in;
                struct {
                    double q;  // charge
                    vec_t p_bar;  // electric dipole moment
                    vec_t m_bar;  // magnetic dipole moment
                } em;
            } u;
        } obj_lst[MAX_OBJ_COUNT];
    } ch;
    struct in_s {
        struct {
            double m_dot;  // mass flow rate
            vec_t F_bar;  // force vector
            vec_t M_bar;  // torque vector
            vec_t v_dot;  // force vector
            vec_t om_dot;  // torque vector
        } sys;
        struct {
            double m_dot;  // mass flow rate
            vec_t F_bar;  // force vector
            vec_t M_bar;  // torque vector
        } obj_lst[MAX_OBJ_COUNT];
    } in;
    struct out_s {
        struct {
            double m;  // mass
            vec_t c_bar;  // center of mass
            mat_t I_cm;  // moment of inertia
        } sys;
    } out;
    struct em_s {
        struct {
            double q;  // charge
            vec_t p_bar;  // electric dipole moment
            vec_t m_bar;  // magnetic dipole moment
            vec_t E_bar;  // electric field
            vec_t B_bar;  // magnetic field
        } sys;
        struct {
            double q;  // charge
            vec_t p_bar;  // electric dipole moment
            vec_t m_bar;  // magnetic dipole moment
        } obj_lst[MAX_OBJ_COUNT];
    } em;
};

#endif  // __VEHICLE_MODEL_H__

