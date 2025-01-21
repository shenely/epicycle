# built-in libraries
import math

# external libraries
import numpy
import numpy.ctypeslib
import scipy.linalg
import pytest

# internal libraries
from epicycle import vec
from epicycle import quat
from epicycle import gee
from epicycle.vehicle_model import *


def test_gee_point_mass():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(numpy.array([1.0, 0.0, 0.0])),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(numpy.array([-8.0, 0.0, 0.0])),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(30.0)),
                    math.sin(math.radians(30.0)),
                    0.0
                ]) * 7.0e6
            ),
            q=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(60.0 / 2)),
                    0.0,
                    0.0,
                    math.sin(math.radians(60.0 / 2))
                ])
            ),
        ),
        obj_lst=(
            st_t.obj_t(m=8.0),
            st_t.obj_t(m=1.0),
        ),
    )
    in_ = in_t()
    gee.gee(2, cfg, st, in_)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    M_bar = numpy.ctypeslib.as_array(in_.sys.M_bar)
    assert math.isclose(F_bar[0], - 4.5 * gee.G_MU / 49.0e12 * math.sqrt(3.0))
    assert math.isclose(F_bar[1], - 4.5 * gee.G_MU / 49.0e12)
    assert math.isclose(F_bar[2], 0.0)
    assert math.isclose(M_bar[0], 0.0)
    assert math.isclose(M_bar[1], 0.0)
    assert math.isclose(
        M_bar[2], - 54.0 * gee.G_MU / 343.0e18 * math.sqrt(3.0),
        abs_tol=1.48e-8
    )


def test_gee_rigid_body():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(numpy.array([0.0, 0.0, 0.0])),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(30.0)),
                    math.sin(math.radians(30.0)),
                    0.0
                ]) * 7.0e6
            ),
            q=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(60.0 / 2)),
                    0.0,
                    0.0,
                    math.sin(math.radians(60.0 / 2))
                ])
            ),
        ),
        obj_lst=(
            st_t.obj_t(
                m=9.0,
                I_cm=numpy.ctypeslib.as_ctypes(numpy.array([0.0, 72.0, 72.0])),
            ),
        ),
    )
    in_ = in_t()
    gee.gee(1, cfg, st, in_)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    M_bar = numpy.ctypeslib.as_array(in_.sys.M_bar)
    assert math.isclose(F_bar[0], - 4.5 * gee.G_MU / 49.0e12 * math.sqrt(3.0))
    assert math.isclose(F_bar[1], - 4.5 * gee.G_MU / 49.0e12)
    assert math.isclose(F_bar[2], 0.0)
    assert math.isclose(M_bar[0], 0.0)
    assert math.isclose(M_bar[1], 0.0)
    assert math.isclose(
        M_bar[2], - 54.0 * gee.G_MU / 343.0e18 * math.sqrt(3.0)
    )


def test_gee_bad():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
        obj_lst=(
            st_t.obj_t(m=1.0),
        ),
    )
    in_ = in_t()
    with pytest.raises(ZeroDivisionError):
        gee.gee(1, cfg, st, in_)


def test_gee_fast():
    st = st_t(
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(30.0)),
                    math.sin(math.radians(30.0)),
                    0.0
                ]) * 7.0e6
            ),
            q=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(60.0 / 2)),
                    0.0,
                    0.0,
                    math.sin(math.radians(60.0 / 2))
                ])
            ),
        ),
    )
    in_ = in_t()
    out = out_t(
        sys=out_t.sys_t(
            m=9.0,
            c_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
            I_cm=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    [0.0, 0.0, 0.0],
                    [0.0, 72.0, 0.0],
                    [0.0, 0.0, 72.0]
                ])
            ),
        ),
    )
    gee.gee_fast(st, in_, out)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    M_bar = numpy.ctypeslib.as_array(in_.sys.M_bar)
    assert math.isclose(F_bar[0], - 4.5 * gee.G_MU / 49.0e12 * math.sqrt(3.0))
    assert math.isclose(F_bar[1], - 4.5 * gee.G_MU / 49.0e12)
    assert math.isclose(F_bar[2], 0.0)
    assert math.isclose(M_bar[0], 0.0)
    assert math.isclose(M_bar[1], 0.0)
    assert math.isclose(
        M_bar[2], - 54.0 * gee.G_MU / 343.0e18 * math.sqrt(3.0)
    )

def test_geoall():
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
    em = em_t()
    gee.geoall(st, in_, out, em)
    r_bar = numpy.ctypeslib.as_array(st.sys.r_bar)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    B_bar = numpy.ctypeslib.as_array(em.sys.B_bar)
    print(F_bar)
    print(B_bar)
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
    # H = math.sqrt(B_bar[0] ** 2 + B_bar[1] ** 2)
    # F = scipy.linalg.norm(B_bar)
    assert math.isclose(F, G, rel_tol=1e-2)
    assert math.isclose(numpy.dot(F_hat, G_hat), 1.0, rel_tol=1e-2)
    # assert math.isclose(F_bar[0], G_bar[0], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[1], G_bar[1], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[2], G_bar[2], rel_tol=1.22e-4)
    assert not numpy.any(numpy.isnan(B_bar))
    # assert math.isclose(H / 24377.2e-9, 1.0, rel_tol=1e-2)
    # assert math.isclose(F / 55348.7e-9, 1.0, rel_tol=1e-2)

def test_geoall_node():
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
    em = em_t()
    gee.geoall(st, in_, out, em)
    r_bar = numpy.ctypeslib.as_array(st.sys.r_bar)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    B_bar = numpy.ctypeslib.as_array(em.sys.B_bar)
    print(F_bar)
    print(B_bar)
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
    # H = math.sqrt(B_bar[0] ** 2 + B_bar[1] ** 2)
    # F = scipy.linalg.norm(B_bar)
    assert math.isclose(F, G, rel_tol=2e-2)
    assert math.isclose(numpy.dot(F_hat, G_hat), 1.0, rel_tol=1e-2)
    # assert math.isclose(F_bar[0], G_bar[0], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[1], G_bar[1], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[2], G_bar[2], rel_tol=1.22e-4)
    assert not numpy.any(numpy.isnan(B_bar))
    # assert math.isclose(H / 24377.2e-9, 1.0, rel_tol=1e-2)
    # assert math.isclose(F / 55348.7e-9, 1.0, rel_tol=1e-2)

def test_geoall_pole():
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
    em = em_t()
    gee.geoall(st, in_, out, em)
    r_bar = numpy.ctypeslib.as_array(st.sys.r_bar)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    B_bar = numpy.ctypeslib.as_array(em.sys.B_bar)
    print(F_bar)
    print(B_bar)
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
    # H = math.sqrt(B_bar[0] ** 2 + B_bar[1] ** 2)
    # F = scipy.linalg.norm(B_bar)
    assert math.isclose(F, G, rel_tol=1e-2)
    assert math.isclose(numpy.dot(F_hat, G_hat), 1.0, rel_tol=1e-2)
    # assert math.isclose(F_bar[0], G_bar[0], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[1], G_bar[1], rel_tol=1.22e-4)
    # assert math.isclose(F_bar[2], G_bar[2], rel_tol=1.22e-4)
    assert not numpy.any(numpy.isnan(B_bar))
    # assert math.isclose(H / 24377.2e-9, 1.0, rel_tol=1e-2)
    # assert math.isclose(F / 55348.7e-9, 1.0, rel_tol=1e-2)

