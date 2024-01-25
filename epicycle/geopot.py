# built-in libraries
import ctypes

# external libraries
# ...

# internal libraries
from . import libgee
from .vec import p_vec_t
from .data_model import (
    p_cfg_t,
    st_t, p_st_t,
    in_t, p_in_t,
    out_t, p_out_t,
    p_em_t,
)

# exports
__all__ = ("eval", "geopot")

# constants
# ...


# void geopot_eval(double, vec_t*, vec_t*)
libgee.geopot_eval.argtypes = [ctypes.c_double, p_vec_t, p_vec_t]
libgee.geopot_eval.restype = ctypes.c_bool
def eval(m: float, r_bar):
    F_bar = numpy.empty((3,), dtype=numpy.float64)
    if not libgee.geopot_eval(m, r_bar, F_bar):
        raise ZeroDivisionError
    return F_bar


# bool geopot(size_t, struct cfg_s*, struct st_s*,
#             struct in_s*, struct out_s*, struct em_s*)
libgee.geopot.argtypes = [
    ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t, p_em_t]
libgee.geopot.restype = ctypes.c_bool
def geopot(st: st_t, in_: in_t, out: out_t):
    if not libgee.geopot(
        0,
        None,
        ctypes.byref(st),
        ctypes.byref(in_),
        ctypes.byref(out),
        None
    ):
        raise ZeroDivisionError

