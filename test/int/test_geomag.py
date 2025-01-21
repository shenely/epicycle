# built-in libraries
import math

# external libraries
import numpy
import numpy.ctypeslib
import scipy.linalg

# internal libraries
from epicycle import quat
from epicycle import geomag
from epicycle.vehicle_model import *


def test_geomag():
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(30.0)) ** 2,
                    math.sin(math.radians(60.0)) / 2,
                    math.sin(math.radians(30.0))
                ]) * 7.0e6
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
    )
    out = out_t()
    em = em_t()
    geomag.geomag(st, out, em)
    B_bar = numpy.ctypeslib.as_array(em.sys.B_bar)
    print(B_bar)
    H = math.sqrt(B_bar[0] ** 2 + B_bar[1] ** 2)
    F = scipy.linalg.norm(B_bar)
    assert not numpy.any(numpy.isnan(B_bar))
    # assert math.isclose(H / 24377.2e-9, 1.0, rel_tol=1e-2)
    # assert math.isclose(F / 55348.7e-9, 1.0, rel_tol=1e-2)


def test_geomag_node():
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7.0e6, 0.0, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
    )
    out = out_t()
    em = em_t()
    geomag.geomag(st, out, em)
    B_bar = numpy.ctypeslib.as_array(em.sys.B_bar)
    print(B_bar)
    H = math.sqrt(B_bar[0] ** 2 + B_bar[1] ** 2)
    F = scipy.linalg.norm(B_bar)
    assert not numpy.any(numpy.isnan(B_bar))
    # assert math.isclose(H / 24377.2e-9, 1.0, rel_tol=1e-2)
    # assert math.isclose(F / 55348.7e-9, 1.0, rel_tol=1e-2)


def test_geomag_pole():
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 7.0e6])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
    )
    out = out_t()
    em = em_t()
    geomag.geomag(st, out, em)
    B_bar = numpy.ctypeslib.as_array(em.sys.B_bar)
    print(B_bar)
    H = math.sqrt(B_bar[0] ** 2 + B_bar[1] ** 2)
    F = scipy.linalg.norm(B_bar)
    assert not numpy.any(numpy.isnan(B_bar))
    # assert math.isclose(H / 24377.2e-9, 1.0, rel_tol=1e-2)
    # assert math.isclose(F / 55348.7e-9, 1.0, rel_tol=1e-2)

