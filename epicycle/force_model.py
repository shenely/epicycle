# built-in libraries
import ctypes

# external libraries
import numpy

# internal libraries
from . import libcore
from .gvec import gvec_t
from .data_model import (
    cfg_t, p_cfg_t,
    st_t, p_st_t,
    ch_t, p_ch_t,
    in_t, p_in_t,
    out_t, p_out_t,
    em_t, p_em_t,
    tol_t, p_tol_t,
)

# exports
__all__ = (
    "force_model_t", "p_force_model_t",
    "interp_st", "interp_obj_lst",
    "solve_st_dot", "solve_st_delta",
    "solve_ch", "solve_in", "solve_out", "solve_em",
)


class force_model_t(ctypes.Structure):
    _fields_ = [
        ("size", ctypes.c_size_t),
        ("accum_fun", ctypes.c_void_p),
        ("step_fun", ctypes.c_void_p),
        ("abstol", tol_t),
        ("reltol", tol_t),
        ("fun_lst", ctypes.c_void_p * 16),
    ]


p_force_model_t = ctypes.POINTER(force_model_t)


# void interp_st(size_t, struct st_s*, struct st_s*, struct st_s*)
libcore.interp_st.argtypes = [
    ctypes.c_size_t,
    p_st_t, p_st_t, p_st_t
]
def interp_st(size: int, prev: st_t, next: st_t, curr: st_t):
    libcore.interp_st(
        size,
        ctypes.byref(prev),
        ctypes.byref(next),
        ctypes.byref(curr),
    )


# void interp_obj_lst(size_t, struct st_s*, struct st_s*, struct st_s*)
libcore.interp_obj_lst.argtypes = [
    ctypes.c_size_t,
    p_st_t, p_st_t, p_st_t
]
def interp_obj_lst(size: int, prev: st_t, next: st_t, curr: st_t):
    libcore.interp_obj_lst(
        size,
        ctypes.byref(prev),
        ctypes.byref(next),
        ctypes.byref(curr),
    )


# void solve_st_dot(
#   size_t, struct cfg_s*, struct st_s*, struct st_s*, struct in_s*)
libcore.solve_st_dot.argtypes = [
    ctypes.c_size_t,
    p_cfg_t, p_st_t, p_st_t, p_in_t
]
def solve_st_dot(size: int, cfg: cfg_t, prev: st_t, next: st_t, in_: in_t):
    libcore.solve_st_dot(
        size,
        ctypes.byref(cfg),
        ctypes.byref(prev),
        ctypes.byref(next),
        ctypes.byref(in_),
    )


# void solve_st_delta(
#   size_t, struct cfg_s*, struct st_s*, struct st_s*, struct out_s*)
libcore.solve_st_delta.argtypes = [
    ctypes.c_size_t,
    p_cfg_t, p_st_t, p_st_t, p_out_t
]
def solve_st_delta(size: int, cfg: cfg_t, prev: st_t, next: st_t, out: out_t):
    libcore.solve_st_delta(
        size,
        ctypes.byref(cfg),
        ctypes.byref(prev),
        ctypes.byref(next),
        ctypes.byref(out)
    )


# void solve_ch(
#   size_t, struct ch_s*,
#   struct st_s*, struct st_s*, struct in_s*, struct em_s*)
libcore.solve_ch.argtypes = [
    ctypes.c_size_t,
    p_cfg_t, p_ch_t, p_st_t, p_st_t, p_in_t, p_em_t
]
libcore.solve_ch.restype = ctypes.c_bool
def solve_ch(
    size: int,
    cfg: cfg_t,
    ch: ch_t,
    prev: st_t,
    next: st_t,
    in_: in_t,
    em: em_t
) -> bool:
    return libcore.solve_ch(
        size,
        ctypes.byref(cfg),
        ctypes.byref(ch),
        ctypes.byref(prev) if prev else None,
        ctypes.byref(next) if next else None,
        ctypes.byref(in_) if in_ else None,
        ctypes.byref(em) if em else None,
    )


# void solve_in(
#   size_t, struct cfg_s*, struct st_s*, struct in_s*, struct out_s*)
libcore.solve_in.argtypes = [ctypes.c_size_t, p_cfg_t, p_st_t, p_in_t, p_out_t]
def solve_in(size: int, cfg: cfg_t, st: st_t, in_: in_t, out: out_t):
    libcore.solve_in(
        size,
        ctypes.byref(cfg),
        ctypes.byref(st),
        ctypes.byref(in_),
        ctypes.byref(out)
    )


# void solve_out(size_t, struct cfg_s*, struct st_s*, struct out_s*)
libcore.solve_out.argtypes = [
    ctypes.c_size_t,
    p_cfg_t, p_st_t, p_out_t
]
def solve_out(size: int, cfg: cfg_t, st: st_t, out: out_t):
    libcore.solve_out(
        size,
        ctypes.byref(cfg),
        ctypes.byref(st),
        ctypes.byref(out),
    )


# void solve_em(size_t, struct cfg_s*, struct em_s*)
libcore.solve_em.argtypes = [ctypes.c_size_t, p_cfg_t, p_em_t]
def solve_em(size: int, cfg: cfg_t, em: em_t):
    libcore.solve_em(
        size,
        ctypes.byref(cfg),
        ctypes.byref(em),
    )

