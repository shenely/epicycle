# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, p_vec_t, mat_t, p_mat_t

# exports
__all__ = (
    "mat_t", "p_mat_t",
    "eye",
    "neg",
    "tr", "det", "inv", "T",
    "add", "sub",
    "muls", "mul", "mulv", "vmul",
)


# void mat_eye(mat_t*)
libmath.mat_eye.argtypes = [p_mat_t]
def eye():
    A = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat_eye(A)
    return A


# void mat_neg(mat_t*, mat_t*)
libmath.mat_neg.argtypes = [p_mat_t, p_mat_t]
def neg(A):
    B = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat_neg(A, B)
    return B


# double mat_tr(mat_t*)
libmath.mat_tr.argtypes = [p_mat_t]
libmath.mat_tr.restype = ctypes.c_double
def tr(A):
    return libmath.mat_tr(A)


# double mat_det(mat_t*)
libmath.mat_det.argtypes = [p_mat_t]
libmath.mat_det.restype = ctypes.c_double
def det(A):
    return libmath.mat_det(A)


# bool mat_inv(mat_t*, mat_t*)
libmath.mat_inv.argtypes = [p_mat_t, p_mat_t]
libmath.mat_inv.restype = ctypes.c_bool
def inv(A):
    A_inv = numpy.empty((3, 3), dtype=numpy.float64)
    if not libmath.mat_inv(A, A_inv):
        raise ZeroDivisionError
    return A_inv


# void mat__T(mat_t*, mat_t*)
libmath.mat__T.argtypes = [p_mat_t, p_mat_t]
def T(A):
    A__T = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat__T(A, A__T)
    return A__T


# void mat_add(mat_t*, mat_t*, mat_t*)
libmath.mat_add.argtypes = [p_mat_t, p_mat_t, p_mat_t]
def add(A, B):
    C = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat_add(A, B, C)
    return C


# void mat_sub(mat_t*, mat_t*, mat_t*)
libmath.mat_sub.argtypes = [p_mat_t, p_mat_t, p_mat_t]
def sub(A, B):
    C = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat_sub(A, B, C)
    return C


# void mat_muls(mat_t*, double, mat_t*)
libmath.mat_muls.argtypes = [p_mat_t, ctypes.c_double, p_mat_t]
def muls(A, s):
    B = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat_muls(A, s, B)
    return B


# void mat_mul(mat_t*, mat_t*, mat_t*)
libmath.mat_mul.argtypes = [p_mat_t, p_mat_t, p_mat_t]
def mul(A, B):
    C = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.mat_mul(A, B, C)
    return C


# void mat_mulv(mat_t*, vec_t*, vec_t*)
libmath.mat_mulv.argtypes = [p_mat_t, p_vec_t, p_vec_t]
def mulv(A, u_bar):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.mat_mulv(A, u_bar, v_bar)
    return v_bar


# void mat_vmul(vec_t*, mat_t*, vec_t*)
libmath.mat_vmul.argtypes = [p_vec_t, p_mat_t, p_vec_t]
def vmul(u_bar, A):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.mat_vmul(u_bar, A, v_bar)
    return v_bar

