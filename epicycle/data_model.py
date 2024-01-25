# built-in libraries
import ctypes
import enum

# external libraries
import numpy

# internal libraries
from . import MAX_OBJ_COUNT
from .vec import vec_t
from .quat import quat_t
from .mat import mat_t
from .dmat import dmat_t
from .gvec import gvec_t

# exports
__all__ = (
    "cfg_t", "p_cfg_t",
    "st_t", "p_st_t",
    "ch_t", "p_ch_t",
    "in_t", "p_in_t",
    "out_t", "p_out_t",
    "em_t", "p_em_t",
    "data_model_t", "p_data_model_t",
    "tol_t", "p_tol_t",
)


class cfg_t(ctypes.Structure):

    class clk_t(ctypes.Structure):
        _fields_ = [
            ("delta_t", ctypes.c_double),
        ]
        
    class obj_t(ctypes.Structure):
        _fields_ = [
            ("bbox", dmat_t),
            ("r_bar", vec_t),
            ("q", quat_t),
        ]
    
    _fields_ = [
        ("clk", clk_t),
        ("obj_lst", obj_t * MAX_OBJ_COUNT),
    ]


class st_t(ctypes.Structure):

    class clk_t(ctypes.Structure):
        _fields_ = [
            ("n", ctypes.c_ulonglong),
            ("t", ctypes.c_double),
        ]
        
    class sys_t(ctypes.Structure):
        _fields_ = [
            ("r_bar", vec_t),
            ("q", quat_t),
            ("v_bar", vec_t),
            ("om_bar", vec_t),
        ]
        
    class obj_t(ctypes.Structure):
        _fields_ = [
            ("m", ctypes.c_double),
            ("I_cm", dmat_t),
            ("p_bar", vec_t),
            ("h_bar", vec_t),
        ]
    
    _fields_ = [
        ("clk", clk_t),
        ("sys", sys_t),
        ("obj_lst", obj_t * MAX_OBJ_COUNT),
    ]


class ch_t(ctypes.Structure):

    class clk_t(ctypes.Structure):
        _fields_ = [
            ("t", ctypes.c_double),
        ]
        
    class obj_t(ctypes.Structure):

        class _T(enum.IntEnum):
            E_NA = 0
            E_ST = enum.auto()
            E_IN = enum.auto()
            E_EM = enum.auto()

        class _U(ctypes.Union):

            class st_t(ctypes.Structure):
                _fields_ = [
                    ("m", ctypes.c_double),
                    ("p_bar", vec_t),
                    ("h_bar", vec_t),
                ]

            class in_t(ctypes.Structure):
                _fields_ = [
                    ("m_dot", ctypes.c_double),
                    ("F_bar", vec_t),
                    ("M_bar", vec_t),
                ]

            class em_t(ctypes.Structure):
                _fields_ = [
                    ("q", ctypes.c_double),
                    ("p_bar", vec_t),
                    ("m_bar", vec_t),
                ]

            _fields_ = [
                ("st", st_t),
                ("in_", in_t),
                ("em", em_t),
            ]

        _anonymous_ = ("U",)
        _fields_ = [
            ("T", ctypes.c_int),
            ("U", _U),
        ]
    
    _fields_ = [
        ("clk", clk_t),
        ("obj_lst", obj_t * MAX_OBJ_COUNT),
    ]


class in_t(ctypes.Structure):
        
    class sys_t(ctypes.Structure):
        _fields_ = [
            ("m_dot", ctypes.c_double),
            ("F_bar", vec_t),
            ("M_bar", vec_t),
            ("v_dot", vec_t),
            ("om_dot", vec_t),
        ]
        
    class obj_t(ctypes.Structure):
        _fields_ = [
            ("m_dot", ctypes.c_double),
            ("F_bar", vec_t),
            ("M_bar", vec_t),
        ]
    
    _fields_ = [
        ("sys", sys_t),
        ("obj_lst", obj_t * MAX_OBJ_COUNT),
    ]


class out_t(ctypes.Structure):
        
    class sys_t(ctypes.Structure):
        _fields_ = [
            ("m", ctypes.c_double),
            ("c_bar", vec_t),
            ("I_cm", mat_t),
        ]
    
    _fields_ = [
        ("sys", sys_t),
    ]


class em_t(ctypes.Structure):
        
    class sys_t(ctypes.Structure):
        _fields_ = [
            ("q", ctypes.c_double),
            ("p_bar", vec_t),
            ("m_bar", vec_t),
            ("E_bar", vec_t),
            ("B_bar", vec_t),
        ]
        
    class obj_t(ctypes.Structure):
        _fields_ = [
            ("q", ctypes.c_double),
            ("p_bar", vec_t),
            ("m_bar", vec_t),
        ]
    
    _fields_ = [
        ("sys", sys_t),
        ("obj_lst", obj_t * MAX_OBJ_COUNT),
    ]


class data_model_t(ctypes.Structure):
    _fields_ = [
        ("size", ctypes.c_size_t),
        ("cfg", cfg_t),
        ("st", st_t),
        ("ch", ch_t),
        ("in_", in_t),
        ("out", out_t),
        ("em", em_t),
    ]


class tol_t(ctypes.Union):
    _anonymous_ = ("S",)
    _fields_ = [
        ("S", st_t.sys_t),
        ("gvec", gvec_t),
    ]


p_cfg_t = ctypes.POINTER(cfg_t)
p_st_t = ctypes.POINTER(st_t)
p_ch_t = ctypes.POINTER(ch_t)
p_in_t = ctypes.POINTER(in_t)
p_out_t = ctypes.POINTER(out_t)
p_em_t = ctypes.POINTER(em_t)
p_data_model_t = ctypes.POINTER(data_model_t)
p_tol_t = ctypes.POINTER(tol_t)

