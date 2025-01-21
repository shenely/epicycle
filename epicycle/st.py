# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libcore
from .vehicle_model import st_t

# exports
__all__ = (
    "st_t", "p_st_t",
    "neg",
    "norm", "unit"
    "add", "sub",
    "muls", "dot", "omul",
)

# data types
st_t = st_t.sys_t
p_st_t = ctypes.POINTER(st_t)


# void st_neg(st_t*, st_t*)
libcore.st_neg.argtypes = [p_st_t, p_st_t]
def neg(a):
    b = st_t()
    libcore.st_neg(ctypes.byref(a), ctypes.byref(b))
    return b


# void st_add(st_t*, st_t*, st_t*)
libcore.st_add.argtypes = [p_st_t, p_st_t, p_st_t]
def add(a, b):
    c = st_t()
    libcore.st_add(ctypes.byref(a), ctypes.byref(b), ctypes.byref(c))
    return c


# void st_sub(st_t*, st_t*, st_t*)
libcore.st_sub.argtypes = [p_st_t, p_st_t, p_st_t]
def sub(a, b):
    c = st_t()
    libcore.st_sub(ctypes.byref(a), ctypes.byref(b), ctypes.byref(c))
    return c


# void st_muls(st_t*, double, st_t*)
libcore.st_muls.argtypes = [p_st_t, ctypes.c_double, p_st_t]
def muls(u_bar, s):
    b = st_t()
    libcore.st_muls(ctypes.byref(a), s, ctypes.byref(b))
    return b

