# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, p_mat_t, dmat_t, p_dmat_t

# exports
__all__ = (
    "dmat_t", "p_dmat_t",
    "eye",
    "tr", "det", "inv",
    "mul", "muld", "dmul",
)


# double dmat_tr(dmat_t*)
libmath.dmat_eye.argtypes = [p_dmat_t]
def eye():
    A = numpy.empty((3,), dtype=numpy.float64)
    libmath.dmat_eye(A)
    return A


# double dmat_tr(dmat_t*)
libmath.dmat_tr.argtypes = [p_dmat_t]
libmath.dmat_tr.restype = ctypes.c_double
def tr(A):
    return libmath.dmat_tr(A)


# double dmat_det(dmat_t*)
libmath.dmat_det.argtypes = [p_dmat_t]
libmath.dmat_det.restype = ctypes.c_double
def det(A):
    return libmath.dmat_det(A)


# bool dmat_inv(dmat_t*, dmat_t*)
libmath.dmat_inv.argtypes = [p_dmat_t, p_dmat_t]
libmath.dmat_inv.restype = ctypes.c_bool
def inv(A):
    A_inv = numpy.empty((3,), dtype=numpy.float64)
    if not libmath.dmat_inv(A, A_inv):
        raise ZeroDivisionError
    return A_inv


# void dmat_mul(dmat_t*, dmat_t*, dmat_t*)
libmath.dmat_mul.argtypes = [p_dmat_t, p_dmat_t, p_dmat_t]
def mul(A, B):
    C = numpy.empty((3,), dtype=numpy.float64)
    libmath.dmat_mul(A, B, C)
    return C


# void dmat_muld(mat_t*, dmat_t*, mat_t*)
libmath.dmat_muld.argtypes = [p_mat_t, p_dmat_t, p_mat_t]
def muld(A, B):
    C = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.dmat_muld(A, B, C)
    return C


# void dmat_mulv(dmat_t*, mat_t*, mat_t*)
libmath.dmat_dmul.argtypes = [p_dmat_t, p_mat_t, p_mat_t]
def dmul(A, B):
    C = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.dmat_dmul(A, B, C)
    return C

