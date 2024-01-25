# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, vec_t, p_vec_t, p_quat_t, p_mat_t

# exports
__all__ = (
    "vec_t", "p_vec_t",
    "zero",
    "pos", "neg",
    "norm", "unit",
    "iszero", "isunit",
    "add", "sub",
    "muls", "dot", "cross", "omul",
    "cross_mat",
    "exp",
    "rot", "irot",
)


# void vec_zero(vec_t*)
libmath.vec_zero.argtypes = [p_vec_t]
def zero():
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_zero(v_bar)
    return v_bar


# void vec_pos(vec_t*, vec_t*)
libmath.vec_pos.argtypes = [p_vec_t, p_vec_t]
def pos(u_bar):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_pos(u_bar, v_bar)
    return v_bar


# void vec_neg(vec_t*, vec_t*)
libmath.vec_neg.argtypes = [p_vec_t, p_vec_t]
def neg(u_bar):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_neg(u_bar, v_bar)
    return v_bar


# double vec_norm(vec_t*)
libmath.vec_norm.argtypes = [p_vec_t]
libmath.vec_norm.restype = ctypes.c_double
def norm(u_bar):
    return libmath.vec_norm(u_bar)


# bool vec_unit(vec_t*, vec_t*)
libmath.vec_unit.argtypes = [p_vec_t, p_vec_t]
libmath.vec_unit.restype = ctypes.c_bool
def unit(u_bar):
    u_hat = numpy.empty((3,), dtype=numpy.float64)
    if not libmath.vec_unit(u_bar, u_hat):
        raise ZeroDivisionError
    return u_hat


# bool vec_iszero(vec_t*)
libmath.vec_iszero.argtypes = [p_vec_t]
libmath.vec_iszero.restype = ctypes.c_bool
def iszero(v_bar):
    return libmath.vec_iszero(v_bar)


# bool vec_isunit(vec_t*)
libmath.vec_isunit.argtypes = [p_vec_t]
libmath.vec_isunit.restype = ctypes.c_bool
def isunit(v_bar):
    return libmath.vec_isunit(v_bar)


# void vec_add(vec_t*, vec_t*, vec_t*)
libmath.vec_add.argtypes = [p_vec_t, p_vec_t, p_vec_t]
def add(u_bar, v_bar):
    w_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_add(u_bar, v_bar, w_bar)
    return w_bar


# void vec_sub(vec_t*, vec_t*, vec_t*)
libmath.vec_sub.argtypes = [p_vec_t, p_vec_t, p_vec_t]
def sub(u_bar, v_bar):
    w_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_sub(u_bar, v_bar, w_bar)
    return w_bar


# void vec_muls(vec_t*, double, vec_t*)
libmath.vec_muls.argtypes = [p_vec_t, ctypes.c_double, p_vec_t]
def muls(u_bar, s):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_muls(u_bar, s, v_bar)
    return v_bar


# double vec_dot(vec_t*, vec_t*)
libmath.vec_dot.argtypes = [p_vec_t, p_vec_t]
libmath.vec_dot.restype = ctypes.c_double
def dot(u_bar, v_bar):
    return libmath.vec_dot(u_bar, v_bar)


# void vec_cross(vec_t*, vec_t*, vec_t*)
libmath.vec_cross.argtypes = [p_vec_t, p_vec_t, p_vec_t]
def cross(u_bar, v_bar):
    w_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_cross(u_bar, v_bar, w_bar)
    return w_bar


# void vec_omul(vec_t*, vec_t*, mat_t*)
libmath.vec_omul.argtypes = [p_vec_t, p_vec_t, p_mat_t]
def omul(u_bar, v_bar):
    A = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.vec_omul(u_bar, v_bar, A)
    return A


# void vec_cross_mat(vec_t*, mat_t*)
libmath.vec_cross_mat.argtypes = [p_vec_t, p_mat_t]
def cross_mat(v_bar):
    A = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.vec_cross_mat(v_bar, A)
    return A


# void vec_exp(vec_t*, quat_t*)
libmath.vec_exp.argtypes = [p_vec_t, p_quat_t]
def exp(v_bar):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.vec_exp(v_bar, q)
    return q


# void vec_rot(quat_t*, vec_t*, vec_t*)
libmath.vec_rot.argtypes = [p_quat_t, p_vec_t, p_vec_t]
def rot(q, u_bar):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_rot(q, u_bar, v_bar)
    return v_bar


# void vec_irot(quat_t*, vec_t*, vec_t*)
libmath.vec_irot.argtypes = [p_quat_t, p_vec_t, p_vec_t]
def irot(q, u_bar):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.vec_irot(q, u_bar, v_bar)
    return v_bar

