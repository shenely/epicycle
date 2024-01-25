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
    p_in_t,
    out_t, p_out_t,
    em_t, p_em_t,
)

# exports
__all__ = ("eval", "geomag")

# constants
# ...


# void geomag_eval(vec_t*, vec_t*)
libgee.geomag_eval.argtypes = [p_vec_t, p_vec_t]
libgee.geomag_eval.restype = ctypes.c_bool
def eval(r_bar):
    B_bar = numpy.empty((3,), dtype=numpy.float64)
    if not libgee.geomag_eval(r_bar, B_bar):
        raise ZeroDivisionError
    return B_bar


# bool geomag(size_t, struct cfg_s*, struct st_s*,
#             struct in_s*, struct out_s*, struct em_s*)
libgee.geomag.argtypes = [
    ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t, p_em_t]
libgee.geomag.restype = ctypes.c_bool
def geomag(st: st_t, out: out_t, em: em_t):
    if not libgee.geomag(
        0,
        None,
        ctypes.byref(st),
        None,
        ctypes.byref(out),
        ctypes.byref(em)
    ):
        raise ZeroDivisionError

