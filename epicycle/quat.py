# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libmath, p_vec_t, p_mat_t, quat_t, p_quat_t

# exports
__all__ = (
    "quat_t", "p_quat_t",
    "one",
    "neg", "conj",
    "norm", "inv", "unit",
    "add", "sub",
    "muls", "mulv", "vmul", "mul",
    "pow", "log",
    "rot_mat", "irot_mat",
)


# void quat_one(quat_t*)
libmath.quat_one.argtypes = [p_quat_t]
def one():
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_one(q)
    return q

# void quat_neg(quat_t*, quat_t*)
libmath.quat_neg.argtypes = [p_quat_t, p_quat_t]
def neg(p):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_neg(p, q)
    return q


# bool quat_conj(quat_t*, quat_t*)
libmath.quat_conj.argtypes = [p_quat_t, p_quat_t]
def conj(p):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_conj(p, q)
    return q


# double quat_norm(quat_t*)
libmath.quat_norm.argtypes = [p_quat_t]
libmath.quat_norm.restype = ctypes.c_double
def norm(q):
    return libmath.quat_norm(q)


# bool quat_inv(quat_t*, quat_t*)
libmath.quat_inv.argtypes = [p_quat_t, p_quat_t]
libmath.quat_inv.restype = ctypes.c_bool
def inv(q):
    q_inv = numpy.empty((4,), dtype=numpy.float64)
    if not libmath.quat_inv(q, q_inv):
        raise ZeroDivisionError
    return q_inv


# bool quat_unit(quat_t*, quat_t*)
libmath.quat_unit.argtypes = [p_quat_t, p_quat_t]
libmath.quat_unit.restype = ctypes.c_bool
def unit(p):
    q = numpy.empty((4,), dtype=numpy.float64)
    if not libmath.quat_unit(p, q):
        raise ZeroDivisionError
    return q


# void quat_add(quat_t*, quat_t*, quat_t*)
libmath.quat_add.argtypes = [p_quat_t, p_quat_t, p_quat_t]
def add(p, q):
    r = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_add(p, q, r)
    return r


# void quat_sub(quat_t*, quat_t*, quat_t*)
libmath.quat_sub.argtypes = [p_quat_t, p_quat_t, p_quat_t]
def sub(p, q):
    r = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_sub(p, q, r)
    return r


# void quat_muls(quat_t*, double, quat_t*)
libmath.quat_muls.argtypes = [p_quat_t, ctypes.c_double, p_quat_t]
def muls(p, s):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_muls(p, s, q)
    return q


# void quat_mulv(quat_t*, vec_t*, quat_t*)
libmath.quat_mulv.argtypes = [p_quat_t, p_vec_t, p_quat_t]
def mulv(p, v_bar):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_mulv(p, v_bar, q)
    return q


# void quat_vmul(vec_t*, quat_t*, quat_t*)
libmath.quat_vmul.argtypes = [p_vec_t, p_quat_t, p_quat_t]
def vmul(v_bar, p):
    q = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_vmul(v_bar, p, q)
    return q


# void quat_mul(quat_t*, quat_t*, quat_t*)
libmath.quat_mul.argtypes = [p_quat_t, p_quat_t, p_quat_t]
def mul(p, q):
    r = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_mul(p, q, r)
    return r


# void quat_pow(quat_t*, double, quat_t*)
libmath.quat_pow.argtypes = [p_quat_t, ctypes.c_double, p_quat_t]
def pow(q, t):
    q__t = numpy.empty((4,), dtype=numpy.float64)
    libmath.quat_pow(q, t, q__t)
    return q__t


# void quat_log(quat_t*, vec_t*)
libmath.quat_log.argtypes = [p_quat_t, p_vec_t]
def log(q):
    v_bar = numpy.empty((3,), dtype=numpy.float64)
    libmath.quat_log(q, v_bar)
    return v_bar


# void quat_rot_mat(quat_t*, mat_t*)
libmath.quat_rot_mat.argtypes = [p_quat_t, p_mat_t]
def rot_mat(q):
    R = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.quat_rot_mat(q, R)
    return R


# void quat_irot_mat(quat_t*, mat_t*)
libmath.quat_irot_mat.argtypes = [p_quat_t, p_mat_t]
def irot_mat(q):
    R = numpy.empty((3, 3), dtype=numpy.float64)
    libmath.quat_irot_mat(q, R)
    return R

