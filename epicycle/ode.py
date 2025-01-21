# built-in libraries
import ctypes
import enum
import typing

# external libraries
import numpy

# internal libraries
from . import libcore
from .st import st_t, p_st_t

__all__ = (
    "solve_ivp",
    "solve_ivp_with_euler",
    "solve_ivp_with_verlet",
    "solve_ivp_with_rk4",
    "solve_ivp_with_heuler",
    "solve_ivp_with_dopri",
    "solve_ivp_with_beuler",
    "solve_ivp_with_midp",
    "solve_ivp_with_vgl4",
    "solve_ivp_with_vgl6",
)


# bool solve_ivp(
#     double, const st_t*,
#     double, st_t* restrict,
#     ode_meth_t*, ode_fun_t, ode_step_t, 
#     size_t, ...)
libcore.solve_ivp.argtypes = [
    ctypes.c_double, p_st_t,
    ctypes.c_double, p_st_t,
    ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p,
    ctypes.c_size_t
]
libcore.solve_ivp.restype = ctypes.c_bool
def solve_ivp(
    x0: float, y0, x1: float, y1,
    meth: typing.Optional[typing.Any],
    fun, step, nargs, *vargs
) -> bool:
    y0 = st_t.from_buffer(y0)
    y1 = st_t.from_buffer(y1)
    return libcore.solve_ivp(
        x0, ctypes.byref(y0), x1, ctypes.byref(y1),
        meth, fun, step,
        nargs, *vargs
    )


def solve_ivp_with_euler(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_euler
    return solve_ivp(x0, y0, x1, y1, meth, fun, None, nargs, *vargs)


def solve_ivp_with_verlet(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_verlet
    return solve_ivp(x0, y0, x1, y1, meth, fun, None, nargs, *vargs)


def solve_ivp_with_rk4(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_rk4
    return solve_ivp(x0, y0, x1, y1, meth, fun, None, nargs, *vargs)


def solve_ivp_with_heuler(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_heuler
    return solve_ivp(
        x0, y0, x1, y1,
        meth, fun, libcore.adjust_time_step,
        nargs, *vargs
    )

def solve_ivp_with_dopri(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_dopri
    return solve_ivp(
        x0, y0, x1, y1,
        meth, fun, libcore.adjust_time_step,
        nargs, *vargs
    )


def solve_ivp_with_beuler(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_beuler
    return solve_ivp(
        x0, y0, x1, y1,
        meth, fun, libcore.adjust_time_step,
        nargs, *vargs
    )


def solve_ivp_with_midp(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_midp
    return solve_ivp(x0, y0, x1, y1, meth, fun, None, nargs, *vargs)


def solve_ivp_with_vgl4(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_vgl4
    return solve_ivp(x0, y0, x1, y1, meth, fun, None, nargs, *vargs)


def solve_ivp_with_vgl6(
    x0: float, y0: st_t,
    x1: float, y1: st_t,
    fun, nargs, *vargs
) -> bool:
    meth = libcore._ode_vgl6
    return solve_ivp(x0, y0, x1, y1, meth, fun, None, nargs, *vargs)
    
