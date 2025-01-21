# built-in libraries
import math

# external libraries
import numpy
import numpy.ctypeslib
import scipy.linalg

# internal libraries
from epicycle import quat
from epicycle import gee
from epicycle import geopot
from epicycle.vehicle_model import *


def test_geopot():
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
    in_ = in_t()
    out = out_t(
        sys=out_t.sys_t(m=1.0),
    )
    geopot.geopot(st, in_, out)
    r_bar = numpy.ctypeslib.as_array(st.sys.r_bar)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    print(F_bar)
    x, y, z = r_bar
    r = 7e6
    G2_bar = gee.G_J2 * numpy.array([
        x * (6 * z ** 2 - 1.5 * (x ** 2 + y ** 2)),
        y * (6 * z ** 2 - 1.5 * (x ** 2 + y ** 2)),
        z * (3 * z ** 2 - 4.5 * (x ** 2 + y ** 2))
    ]) / r ** 7
    G3_bar = gee.G_J3 * numpy.array([
        x * z * (10 * z ** 2 - 7.5 * (x ** 2 + y ** 2)),
        y * z * (10 * z ** 2 - 7.5 * (x ** 2 + y ** 2)),
        4 * z ** 2 * (z ** 2 - 3 * (x ** 2 + y ** 2))
        + 1.5 * (x ** 2 + y ** 2) ** 2,
    ]) / r ** 9
    G_bar = G2_bar + G3_bar
    print(G_bar)
    F = scipy.linalg.norm(F_bar)
    F_hat = F_bar / F
    G = scipy.linalg.norm(G_bar)
    G_hat = G_bar / G
    assert math.isclose(F, G, rel_tol=1e-2)
    assert math.isclose(numpy.dot(F_hat, G_hat), 1.0, rel_tol=1e-2)
    # assert math.isclose(F_bar[0], G_bar[0], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[1], G_bar[1], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[2], G_bar[2], rel_tol=1.22e-4)


def test_geopot_node():
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7.0e6, 0.0, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
    )
    in_ = in_t()
    out = out_t(
        sys=out_t.sys_t(m=1.0),
    )
    geopot.geopot(st, in_, out)
    r_bar = numpy.ctypeslib.as_array(st.sys.r_bar)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    print(F_bar)
    x, y, z = r_bar
    r = 7e6
    G2_bar = gee.G_J2 * numpy.array([
        x * (6 * z ** 2 - 1.5 * (x ** 2 + y ** 2)),
        y * (6 * z ** 2 - 1.5 * (x ** 2 + y ** 2)),
        z * (3 * z ** 2 - 4.5 * (x ** 2 + y ** 2))
    ]) / r ** 7
    G3_bar = gee.G_J3 * numpy.array([
        x * z * (10 * z ** 2 - 7.5 * (x ** 2 + y ** 2)),
        y * z * (10 * z ** 2 - 7.5 * (x ** 2 + y ** 2)),
        4 * z ** 2 * (z ** 2 - 3 * (x ** 2 + y ** 2))
        + 1.5 * (x ** 2 + y ** 2) ** 2,
    ]) / r ** 9
    G_bar = G2_bar + G3_bar
    print(G_bar)
    F = scipy.linalg.norm(F_bar)
    F_hat = F_bar / F
    G = scipy.linalg.norm(G_bar)
    G_hat = G_bar / G
    assert math.isclose(F, G, rel_tol=2e-2)
    assert math.isclose(numpy.dot(F_hat, G_hat), 1.0, rel_tol=1e-2)
    # assert math.isclose(F_bar[0], G_bar[0], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[1], G_bar[1], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[2], G_bar[2], rel_tol=1.22e-4)


def test_geopot_pole():
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 7.0e6])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
    )
    in_ = in_t()
    out = out_t(
        sys=out_t.sys_t(m=1.0),
    )
    geopot.geopot(st, in_, out)
    r_bar = numpy.ctypeslib.as_array(st.sys.r_bar)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    print(F_bar)
    x, y, z = r_bar
    r = 7e6
    G2_bar = gee.G_J2 * numpy.array([
        x * (6 * z ** 2 - 1.5 * (x ** 2 + y ** 2)),
        y * (6 * z ** 2 - 1.5 * (x ** 2 + y ** 2)),
        z * (3 * z ** 2 - 4.5 * (x ** 2 + y ** 2))
    ]) / r ** 7
    G3_bar = gee.G_J3 * numpy.array([
        x * z * (10 * z ** 2 - 7.5 * (x ** 2 + y ** 2)),
        y * z * (10 * z ** 2 - 7.5 * (x ** 2 + y ** 2)),
        4 * z ** 2 * (z ** 2 - 3 * (x ** 2 + y ** 2))
        + 1.5 * (x ** 2 + y ** 2) ** 2,
    ]) / r ** 9
    G_bar = G2_bar + G3_bar
    print(G_bar)
    F = scipy.linalg.norm(F_bar)
    F_hat = F_bar / F
    G = scipy.linalg.norm(G_bar)
    G_hat = G_bar / G
    assert math.isclose(F, G, rel_tol=1e-2)
    assert math.isclose(numpy.dot(F_hat, G_hat), 1.0, rel_tol=1e-2)
    # assert math.isclose(F_bar[0], G_bar[0], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[1], G_bar[1], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[2], G_bar[2], rel_tol=1.22e-4)

