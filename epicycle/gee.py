# built-in libraries
import ctypes

# external libraries
# ...

# internal libraries
from . import libgee
from .vec import p_vec_t
from .data_model import (
    cfg_t, p_cfg_t,
    st_t, p_st_t,
    in_t, p_in_t,
    out_t, p_out_t,
    em_t, p_em_t,
)

# exports
__all__ = (
    "G_RMAX", "G_RMIN", "G_INVF", "G_MU", "G_J2", "G_J3", 
    "gee", "gee_fast", "eval_all", "geoall",
)

# constants
G_RMAX = 6378136.3
G_RMIN = 6356766.0
G_INVF = 298.4579673659263
G_MU = 3986004.415E+8
G_J2 = 1.75553e+25
G_J3 = -2.61913e29


# bool gee(size_t, struct cfg_s*, struct st_s*,
#          struct in_s*, struct out_s*, struct em_s*)
libgee.gee.argtypes = [
    ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t, p_em_t]
libgee.gee.restype = ctypes.c_bool
def gee(size: int, cfg: cfg_t, st: st_t, in_: in_t):
    if not libgee.gee(
        size,
        ctypes.byref(cfg),
        ctypes.byref(st),
        ctypes.byref(in_),
        None, None
    ):
        raise ZeroDivisionError


# bool gee_fast(size_t, struct cfg_s*, struct st_s*,
#               struct in_s*, struct out_s*, struct em_s*)
libgee.gee_fast.argtypes = [
    ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t, p_em_t]
libgee.gee_fast.restype = ctypes.c_bool
def gee_fast(st: st_t, in_: in_t, out: out_t):
    if not libgee.gee_fast(
        0,
        None,
        ctypes.byref(st),
        ctypes.byref(in_),
        ctypes.byref(out),
        None
    ):
        raise ZeroDivisionError


# void geoall_eval(vec_t*, vec_t*)
libgee.geoall_eval.argtypes = [ctypes.c_double, p_vec_t, p_vec_t, p_vec_t]
libgee.geoall_eval.restype = ctypes.c_bool
def eval_all(m: float, r_bar):
    F_bar = numpy.empty((3,), dtype=numpy.float64)
    B_bar = numpy.empty((3,), dtype=numpy.float64)
    if not libgee.geoall_eval(m, r_bar, F_bar, B_bar):
        raise ZeroDivisionError
    return F_bar, B_bar


# bool geoall(size_t, struct cfg_s*, struct st_s*,
#             struct in_s*, struct out_s*, struct em_s*)
libgee.geoall.argtypes = [
    ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t, p_em_t]
libgee.geoall.restype = ctypes.c_bool
def geoall(st: st_t, in_: in_t, out: out_t, em: em_t):
    if not libgee.geoall(
        0,
        None,
        ctypes.byref(st),
        ctypes.byref(in_),
        ctypes.byref(out),
        ctypes.byref(em)
    ):
        raise ZeroDivisionError

