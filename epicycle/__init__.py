# built-in libraries
import os.path
import ctypes

# external libraries
import numpy.ctypeslib

# internal libraries
libpath = os.path.dirname(__file__)
libmath = numpy.ctypeslib.load_library("libepimath", libpath)
libcore = numpy.ctypeslib.load_library("libepicore", libpath)
libgee = numpy.ctypeslib.load_library("libepigee", libpath)

# exports
__all__ = (
    "libmath",
    "libcore",
    "libgee",
    "POLY_DEG",
    "GMAT_NDIM",
    "ODE_EULER",
    "MAX_OBJ_COUNT",
    "vec_t", "p_vec_t",
    "mat_t", "p_mat_t",
    "quat_t", "p_quat_t",
    "dmat_t", "p_dmat_t",
    "poly_t", "p_poly_t",
    "atm_t", "p_atm_t",
)

# constants
POLY_DEG = 5
ODE_EULER = False
MAX_OBJ_COUNT = 16

# data types
vec_t = ctypes.c_double * 3
mat_t = vec_t * 3
quat_t = ctypes.c_double * 4
dmat_t = vec_t


class poly_t(ctypes.Structure):
    _fields_ = [
        ("deg", ctypes.c_size_t),
        ("coeff", ctypes.c_double * (POLY_DEG + 1)),
    ]


p_poly_t = ctypes.POINTER(poly_t)
p_vec_t = numpy.ctypeslib.ndpointer(
    dtype=numpy.float64, ndim=1, shape=(3,), flags="C")
p_mat_t = numpy.ctypeslib.ndpointer(
    dtype=numpy.float64, ndim=2, shape=(3, 3), flags="C")
p_quat_t = numpy.ctypeslib.ndpointer(
    dtype=numpy.float64, ndim=1, shape=(4,), flags="C")
p_dmat_t = p_vec_t

