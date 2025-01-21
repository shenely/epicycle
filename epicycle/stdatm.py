# built-in libraries
import ctypes

# external libraries
# ...

# internal libraries
from . import libgee
from .vehicle_model import (
    cfg_t, p_cfg_t,
    st_t, p_st_t,
    in_t, p_in_t,
    p_out_t,
    p_em_t,
)

# exports
__all__ = (
    "A_RSTAR",
    "A_G0", "A_M0", "A_T0", "A_P0",
    "A_G7", "A_M7", "A_T7",
    "atm_t", "p_atm_t",
    "eval", "stdatm",
)

# constants
A_RSTAR = 8.31432
A_G0 = 9.80665
A_M0 = 28.9644e-3
A_T0 = 288.15
A_P0 = 101325.0
A_G7 = 9.5466
A_M7 = 28.95e-3
A_T7 = 186.87


class atm_t(ctypes.Structure):
    _fields_ = [
        ("th", ctypes.c_double),
        ("p", ctypes.c_double),
        ("rho", ctypes.c_double),
    ]


p_atm_t = ctypes.POINTER(atm_t)


# void stdatm_init()
libgee.stdatm_init()


# bool stdatm_eval(struct atm_s*)
libgee.stdatm_eval.argtypes = [ctypes.c_double, p_atm_t]
libgee.stdatm_eval.restype = ctypes.c_bool
def eval(z: float) -> atm_t:
    atm = atm_t()
    if not libgee.stdatm_eval(z, ctypes.byref(atm)):
        raise OverflowError
    return atm


# bool stdatm(size_t, struct cfg_s*, struct st_s*,
#             struct in_s*, struct out_s*, struct em_s*)
libgee.stdatm.argtypes = [
    ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t, p_em_t]
libgee.stdatm.restype = ctypes.c_bool
def stdatm(size: int, cfg: cfg_t, st: st_t, in_: in_t):
    if not libgee.stdatm(
        size,
        ctypes.byref(cfg),
        ctypes.byref(st),
        ctypes.byref(in_),
        None,
        None
    ):
        raise ZeroDivisionError

