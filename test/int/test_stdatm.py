# built-in libraries
import math

# external libraries
import numpy
import numpy.ctypeslib

# internal libraries
from epicycle import vec
from epicycle import quat
from epicycle import gee
from epicycle import stdatm
from epicycle.vehicle_model import *


def test_stdatm():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                bbox=numpy.ctypeslib.as_ctypes(numpy.ones((3,))),
                r_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, gee.G_RMIN + 250e3])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7.5e3, 0.0, 0.0])
            ),
        ),
    )
    in_ = in_t()
    stdatm.stdatm(1, cfg, st, in_)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    D = 0.5 * 6.073e-11 * 2.0 * 7.5e3 ** 2 * 1.0
    print(F_bar, D)
    assert math.isclose(F_bar[0], - D, rel_tol=1.22e-4)
    assert math.isclose(F_bar[1], 0.0)
    assert math.isclose(F_bar[2], 0.0)

