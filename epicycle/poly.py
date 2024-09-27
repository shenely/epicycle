# built-in libraries
import ctypes

# external libraries
# ...

# internal libraries
from . import libmath, POLY_DEG, poly_t, p_poly_t

# exports
__all__ = (
    "poly_t", "p_poly_t",
    "zero",
    "pos", "neg",
    "tr", "det", "inv", "T",
    "add", "sub",
    "mul", "div",
    "diff", "int",
    "eval",
)


# void poly_zero(mat_t*)
libmath.poly_zero.argtypes = [p_poly_t]
def zero():
    P = poly_t(POLY_DEG)
    libmath.poly_zero(P)
    return P


# void poly_pos(poly_t*, poly_t*)
libmath.poly_pos.argtypes = [p_poly_t, p_poly_t]
def pos(P):
    Q = poly_t(POLY_DEG)
    libmath.poly_pos(ctypes.byref(P), ctypes.byref(Q))
    return Q


# void poly_neg(poly_t*, poly_t*)
libmath.poly_neg.argtypes = [p_poly_t, p_poly_t]
def neg(P):
    Q = poly_t(POLY_DEG)
    libmath.poly_neg(ctypes.byref(P), ctypes.byref(Q))
    return Q


# void poly_add(poly_t*, poly_t*)
libmath.poly_add.argtypes = [p_poly_t, p_poly_t, p_poly_t]
def add(P, Q):
    R = poly_t(POLY_DEG)
    libmath.poly_add(ctypes.byref(P), ctypes.byref(Q), ctypes.byref(R))
    return R


# void poly_sub(poly_t*, poly_t*)
libmath.poly_sub.argtypes = [p_poly_t, p_poly_t, p_poly_t]
def sub(P, Q):
    R = poly_t(POLY_DEG)
    libmath.poly_sub(ctypes.byref(P), ctypes.byref(Q), ctypes.byref(R))
    return R


# void poly_mul(poly_t*, poly_t*)
libmath.poly_mul.argtypes = [p_poly_t, p_poly_t, p_poly_t]
def mul(P, Q):
    R = poly_t(POLY_DEG)
    libmath.poly_mul(P, Q, R)
    return R


# void poly_div(poly_t*, poly_t*, poly_t*, poly_t*)
libmath.poly_div.argtypes = [p_poly_t, p_poly_t, p_poly_t, p_poly_t]
def div(N, D):
    Q = poly_t(POLY_DEG)
    R = poly_t(POLY_DEG)
    libmath.poly_div(
        ctypes.byref(N), ctypes.byref(D),
        ctypes.byref(Q), ctypes.byref(R)
    )
    return Q, R


# void poly_diff(poly_t*, poly_t*)
libmath.poly_diff.argtypes = [p_poly_t, p_poly_t]
def diff(P):
    diff_P = poly_t(POLY_DEG)
    libmath.poly_diff(ctypes.byref(P), ctypes.byref(diff_P))
    return diff_P


# void poly_int(poly_t*, poly_t*)
libmath.poly_int.argtypes = [p_poly_t, p_poly_t]
def int(P):
    int_P = poly_t(POLY_DEG)
    libmath.poly_int(ctypes.byref(P), ctypes.byref(int_P))
    return int_P


# void poly_eval(poly_t*, double)
libmath.poly_eval.argtypes = [p_poly_t, ctypes.c_double]
libmath.poly_eval.restype = ctypes.c_double
def eval(P, x):
    return libmath.poly_eval(ctypes.byref(P), x)

