# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, GMAT_NDIM, gvec_t, p_gvec_t, p_gmat_t

# exports
__all__ = (
    "GMAT_NDIM",
    "gvec_t", "p_gvec_t",
    "zero",
    "pos", "neg",
    "iszero", "isunit",
    "norm", "unit"
    "add", "sub",
    "muls", "dot", "omul",
)


# void gvec_zero(gvec_t*)
libmath.gvec_zero.argtypes = [p_gvec_t]
def zero():
    v_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gvec_zero(v_bar)
    return v_bar


# void gvec_pos(gvec_t*, gvec_t*)
libmath.gvec_pos.argtypes = [p_gvec_t, p_gvec_t]
def pos(u_bar):
    v_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gvec_pos(u_bar, v_bar)
    return v_bar


# void gvec_neg(gvec_t*, gvec_t*)
libmath.gvec_neg.argtypes = [p_gvec_t, p_gvec_t]
def neg(u_bar):
    v_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gvec_neg(u_bar, v_bar)
    return v_bar


# double gvec_norm(vec_t*)
libmath.gvec_norm.argtypes = [p_gvec_t]
libmath.gvec_norm.restype = ctypes.c_double
def norm(u_bar):
    return libmath.gvec_norm(u_bar)


# bool gvec_unit(gvec_t*, gvec_t*)
libmath.gvec_unit.argtypes = [p_gvec_t, p_gvec_t]
libmath.gvec_unit.restype = ctypes.c_bool
def unit(v_bar):
    v_hat = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    if not libmath.gvec_unit(v_bar, v_hat):
        raise ZeroDivisionError
    return v_hat


# bool gvec_iszero(gvec_t*)
libmath.gvec_iszero.argtypes = [p_gvec_t]
libmath.gvec_iszero.restype = ctypes.c_bool
def iszero(v_bar):
    return libmath.gvec_iszero(v_bar)


# bool vec_isunit(gvec_t*)
libmath.gvec_isunit.argtypes = [p_gvec_t]
libmath.gvec_isunit.restype = ctypes.c_bool
def isunit(v_bar):
    return libmath.gvec_isunit(q)


# void gvec_add(gvec_t*, gvec_t*, gvec_t*)
libmath.gvec_add.argtypes = [p_gvec_t, p_gvec_t, p_gvec_t]
def add(u_bar, v_bar):
    w_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gvec_add(u_bar, v_bar, w_bar)
    return w_bar


# void gvec_sub(gvec_t*, gvec_t*, gvec_t*)
libmath.gvec_sub.argtypes = [p_gvec_t, p_gvec_t, p_gvec_t]
def sub(u_bar, v_bar):
    w_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gvec_sub(u_bar, v_bar, w_bar)
    return w_bar


# void gvec_muls(gvec_t*, double, gvec_t*)
libmath.gvec_muls.argtypes = [p_gvec_t, ctypes.c_double, p_gvec_t]
def muls(u_bar, s):
    v_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gvec_muls(u_bar, s, v_bar)
    return v_bar


# double gvec_dot(gvec_t*, gvec_t*)
libmath.gvec_dot.argtypes = [p_gvec_t, p_gvec_t]
libmath.gvec_dot.restype = ctypes.c_double
def dot(u_bar, v_bar):
    return libmath.gvec_dot(u_bar, v_bar)


# void gvec_omul(gvec_t*, gvec_t*, gmat_t*)
libmath.gvec_omul.argtypes = [p_gvec_t, p_gvec_t, p_gmat_t]
def omul(u_bar, v_bar):
    A = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gvec_omul(u_bar, v_bar, A)
    return A

