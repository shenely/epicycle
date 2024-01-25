# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, p_vec_t, p_quat_t

# exports
__all__ = (
    "lerp", "vlerp", "slerp",
    "cint", "squad",
)


# void interp_init()
libmath.interp_init()


# double interp_lerp(double, double, double, double, double)
libmath.interp_lerp.argtypes = [
    ctypes.c_double, ctypes.c_double,
    ctypes.c_double, ctypes.c_double,
    ctypes.c_double,
]
libmath.interp_lerp.restype = ctypes.c_double
def lerp(x0, y0, x1, y1, x):
    return libmath.interp_lerp(x0, y0, x1, y1, x)


# void interp_vlerp(double, vec_t*, double, vec_t*, double, vec_t*)
libmath.interp_vlerp.argtypes = [
    ctypes.c_double, p_vec_t,
    ctypes.c_double, p_vec_t,
    ctypes.c_double, p_vec_t,
]
def vlerp(x0, v0_bar, x1, v1_bar, x):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.interp_vlerp(x0, v0_bar, x1, v1_bar, x, v_bar)
    return v_bar


# void interp_slerp(double, quat_t*, double, quat_t*, double, quat_t*)
libmath.interp_slerp.argtypes = [
    ctypes.c_double, p_quat_t,
    ctypes.c_double, p_quat_t,
    ctypes.c_double, p_quat_t,
]
def slerp(x0, q0, x1, q1, x):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.interp_slerp(x0, q0, x1, q1, x, q)
    return q


# void interp_cint(
#   double, vec_t*, vec_t*,
#   double, vec_t*, vec_t*,
#   double, vec_t*, vec_t*
# )
libmath.interp_cint.argtypes = [
    ctypes.c_double, p_vec_t, p_vec_t,
    ctypes.c_double, p_vec_t, p_vec_t,
    ctypes.c_double, p_vec_t, p_vec_t,
]
def cint(x0, p0_bar, m0_bar, x1, p1_bar, m1_bar, x):
    p_bar = numpy.empty((3,), dtype=numpy.float64)
    m_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.interp_cint(
        x0, p0_bar, m0_bar,
        x1, p1_bar, m1_bar,
        x, p_bar, m_bar
    )
    return p_bar, m_bar


# void interp_squad(
#   double, quat_t*, vec_t*,
#   double, quat_t*, vec_t*,
#   double, quat_t*, vec_t*
# )
libmath.interp_squad.argtypes = [
    ctypes.c_double, p_quat_t, p_vec_t,
    ctypes.c_double, p_quat_t, p_vec_t,
    ctypes.c_double, p_quat_t, p_vec_t,
]
def squad(x0, q0, om0_bar, x1, q1, om1_bar, x):
    q = numpy.empty((4,), dtype=numpy.float64)
    om_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.interp_squad(
        x0, q0, om0_bar,
        x1, q1, om1_bar,
        x, q, om_bar
    )
    return q, om_bar

