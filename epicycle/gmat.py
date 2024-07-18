# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, GMAT_NDIM, p_gvec_t, gmat_t, p_gmat_t

# exports
__all__ = (
    "GMAT_NDIM",
    "gmat_t", "p_gmat_t",
    "eye",
    "neg",
    "tr", "T",
    "add", "sub",
    "muls", "mul", "mulv", "vmul",
)


# void gmat_eye(gmat_t*)
libmath.gmat_eye.argtypes = [p_gmat_t]
def eye():
    A = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat_eye(A)
    return A


# void gmat_neg(gmat_t*, gmat_t*)
libmath.gmat_neg.argtypes = [p_gmat_t, p_gmat_t]
def neg(A):
    B = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat_neg(A, B)
    return B


# double gmat_tr(gmat_t*)
libmath.gmat_tr.argtypes = [p_gmat_t]
libmath.gmat_tr.restype = ctypes.c_double
def tr(A):
    return libmath.gmat_tr(A)


# void gmat__T(mat_t*, mat_t*)
libmath.gmat__T.argtypes = [p_gmat_t, p_gmat_t]
def T(A):
    A__T = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat__T(A, A__T)
    return A__T


# void gmat_add(gmat_t*, gmat_t*, gmat_t*)
libmath.gmat_add.argtypes = [p_gmat_t, p_gmat_t, p_gmat_t]
def add(A, B):
    C = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat_add(A, B, C)
    return C


# void gmat_sub(gmat_t*, gmat_t*, gmat_t*)
libmath.gmat_sub.argtypes = [p_gmat_t, p_gmat_t, p_gmat_t]
def sub(A, B):
    C = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat_sub(A, B, C)
    return C


# void gmat_muls(gmat_t*, double, gmat_t*)
libmath.gmat_muls.argtypes = [p_gmat_t, ctypes.c_double, p_gmat_t]
def muls(A, s):
    B = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat_muls(A, s, B)
    return B


# void gmat_mul(gmat_t*, gmat_t*, gmat_t*)
libmath.gmat_mul.argtypes = [p_gmat_t, p_gmat_t, p_gmat_t]
def mul(A, B):
    C = numpy.empty((GMAT_NDIM, GMAT_NDIM), dtype=numpy.float64)
    libmath.gmat_mul(A, B, C)
    return C


# void gmat_mulv(gmat_t*, gvec_t*, gvec_t*)
libmath.gmat_mulv.argtypes = [p_gmat_t, p_gvec_t, p_gvec_t]
def mulv(A, u_bar):
    v_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gmat_mulv(A, u_bar, v_bar)
    return v_bar


# void gmat_vmul(gvec_t*, gmat_t*, gvec_t*)
libmath.gmat_vmul.argtypes = [p_gvec_t, p_gmat_t, p_gvec_t]
def vmul(u_bar, A):
    v_bar = numpy.empty((GMAT_NDIM,), dtype=numpy.float64)
    libmath.gmat_vmul(u_bar, A, v_bar)
    return v_bar

