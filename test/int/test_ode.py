# built-in libraries
import ctypes
import math

# external libraries
import numpy
import numpy.ctypeslib
import pytest

# internal libraries
from epicycle import libcore, libgee, ODE_EULER
from epicycle import vec
from epicycle import quat
from epicycle import ode
from epicycle.gee import G_MU
from epicycle.vehicle_model import *
from epicycle.force_model import force_model_t


mark_ode_euler = pytest.mark.skipif(
    not ODE_EULER, reason="Euler methods not implemented")


def test_solve_ivp():
    cfg = cfg_t(
        clk=cfg_t.clk_t(delta_t=1.0),
        obj_lst=(
            cfg_t.obj_t(
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    prev = st_t(
        clk=st_t.clk_t(t=0.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 0.0, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    next = st_t(
        clk=st_t.clk_t(t=1.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 7.0e3, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    st = st_t()
    in_ = in_t(
        obj_lst=(
            in_t.obj_t(
                M_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 1.0, 1.0]) * math.pi / 18.0 / math.sqrt(3.0)
                ),
            ),
        )
    )
    out = out_t()
    em = em_t()
    force_model = force_model_t(1, fun_lst=(
        ctypes.cast(libgee.gee_fast, ctypes.c_void_p),
    ))
    ode.solve_ivp(
        prev.clk.t, numpy.frombuffer(prev.sys),
        next.clk.t, numpy.frombuffer(next.sys),
        None, libcore.apply_force_model, None,
        9, ctypes.c_size_t(1), ctypes.byref(cfg),
        ctypes.byref(prev), ctypes.byref(next), ctypes.byref(st),
        ctypes.byref(in_), ctypes.byref(out), ctypes.byref(em),
        ctypes.byref(force_model),
    )
    r_bar = numpy.ctypeslib.as_array(next.sys.r_bar)
    q = numpy.ctypeslib.as_array(next.sys.q)
    v_bar = numpy.ctypeslib.as_array(next.sys.v_bar)
    om_bar = numpy.ctypeslib.as_array(next.sys.om_bar)
    print(r_bar)
    print(q)
    print(v_bar)
    print(om_bar)
    assert math.isclose(r_bar[0], 7000.0e3 - 0.5 * G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
    assert math.isclose(r_bar[1], 7.0e3, rel_tol=1.22e-4)
    assert math.isclose(r_bar[2], 0.0, abs_tol=1.48e-8)
    assert math.isclose(q[0], 0.5 * math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(q[1], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(q[2], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(q[3], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(v_bar[0], - G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
    assert math.isclose(v_bar[1], 7.0e3, rel_tol=1.22e-4)
    assert math.isclose(v_bar[2], 0.0, abs_tol=1.48e-8)
    assert math.isclose(om_bar[0], math.pi / 1.5 / math.sqrt(3.0))
    assert math.isclose(om_bar[1], math.pi / 1.5 / math.sqrt(3.0))
    assert math.isclose(om_bar[2], math.pi / 1.5 / math.sqrt(3.0))


@pytest.mark.parametrize("method", (
    pytest.param(ode.solve_ivp_with_euler, marks=mark_ode_euler),
    # ode.solve_ivp_with_verlet,
    ode.solve_ivp_with_rk4,
    pytest.param(ode.solve_ivp_with_heuler, marks=mark_ode_euler),
    # ode.solve_ivp_with_dopri,
    pytest.param(ode.solve_ivp_with_beuler, marks=mark_ode_euler),
    # ode.solve_ivp_with_midp,
    # ode.solve_ivp_with_vgl4,
    # ode.solve_ivp_with_vgl6,
))
def test_solve_ivp_with_bad(method):
    cfg = cfg_t(
        clk=cfg_t.clk_t(delta_t=1.0),
        obj_lst=(
            cfg_t.obj_t(
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    prev = st_t(
        clk=st_t.clk_t(t=0.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 0.0, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    next = st_t(
        clk=st_t.clk_t(t=1.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 7.0e3, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    st = st_t()
    in_ = in_t(
        obj_lst=(
            in_t.obj_t(
                M_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 1.0, 1.0]) * math.pi / 18.0 / math.sqrt(3.0)
                ),
            ),
        )
    )
    out = out_t()
    em = em_t()
    force_model = force_model_t(1, fun_lst=(
        ctypes.cast(libgee.gee_fast, ctypes.c_void_p),
    ))
    method(
        prev.clk.t, numpy.frombuffer(prev.sys),
        next.clk.t, numpy.frombuffer(next.sys),
        libcore.apply_force_model,
        9, ctypes.c_size_t(1), ctypes.byref(cfg),
        ctypes.byref(prev), ctypes.byref(next), ctypes.byref(st),
        ctypes.byref(in_), ctypes.byref(out), ctypes.byref(em),
        ctypes.byref(force_model),
    )
    r_bar = numpy.ctypeslib.as_array(next.sys.r_bar)
    q = numpy.ctypeslib.as_array(next.sys.q)
    v_bar = numpy.ctypeslib.as_array(next.sys.v_bar)
    om_bar = numpy.ctypeslib.as_array(next.sys.om_bar)
    print(r_bar)
    print(q)
    print(v_bar)
    print(om_bar)
    assert math.isclose(r_bar[0], 7000.0e3 - 0.5 * G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
    assert math.isclose(r_bar[1], 7.0e3, rel_tol=1.22e-4)
    assert math.isclose(r_bar[2], 0.0, abs_tol=1.48e-8)
    # assert math.isclose(q[0], 0.5 * math.sqrt(3.0), rel_tol=1.22e-4)
    # assert math.isclose(q[1], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    # assert math.isclose(q[2], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    # assert math.isclose(q[3], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(v_bar[0], - G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
    assert math.isclose(v_bar[1], 7.0e3, rel_tol=1.22e-4)
    assert math.isclose(v_bar[2], 0.0, abs_tol=1.48e-8)
    assert math.isclose(om_bar[0], math.pi / 1.5 / math.sqrt(3.0))
    assert math.isclose(om_bar[1], math.pi / 1.5 / math.sqrt(3.0))
    assert math.isclose(om_bar[2], math.pi / 1.5 / math.sqrt(3.0))


@pytest.mark.parametrize("method", (
    # ode.solve_ivp_with_euler,
    ode.solve_ivp_with_verlet,
    # ode.solve_ivp_with_rk4,
    # ode.solve_ivp_with_heuler,
    ode.solve_ivp_with_dopri,
    # ode.solve_ivp_with_beuler,
    ode.solve_ivp_with_midp,
    ode.solve_ivp_with_vgl4,
    ode.solve_ivp_with_vgl6,
))
def test_solve_ivp_with_good(method):
    cfg = cfg_t(
        clk=cfg_t.clk_t(delta_t=1.0),
        obj_lst=(
            cfg_t.obj_t(
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    prev = st_t(
        clk=st_t.clk_t(t=0.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 0.0, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    next = st_t(
        clk=st_t.clk_t(t=1.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 7.0e3, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    st = st_t()
    in_ = in_t(
        obj_lst=(
            in_t.obj_t(
                M_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 1.0, 1.0]) * math.pi / 18.0 / math.sqrt(3.0)
                ),
            ),
        )
    )
    out = out_t()
    em = em_t()
    force_model = force_model_t(1, fun_lst=(
        ctypes.cast(libgee.gee_fast, ctypes.c_void_p),
    ))
    method(
        prev.clk.t, numpy.frombuffer(prev.sys),
        next.clk.t, numpy.frombuffer(next.sys),
        libcore.apply_force_model,
        9, ctypes.c_size_t(1), ctypes.byref(cfg),
        ctypes.byref(prev), ctypes.byref(next), ctypes.byref(st),
        ctypes.byref(in_), ctypes.byref(out), ctypes.byref(em),
        ctypes.byref(force_model),
    )
    r_bar = numpy.ctypeslib.as_array(next.sys.r_bar)
    q = numpy.ctypeslib.as_array(next.sys.q)
    v_bar = numpy.ctypeslib.as_array(next.sys.v_bar)
    om_bar = numpy.ctypeslib.as_array(next.sys.om_bar)
    print(r_bar)
    print(q)
    print(v_bar)
    print(om_bar)
    assert math.isclose(r_bar[0], 7000.0e3 - 0.5 * G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
    assert math.isclose(r_bar[1], 7.0e3, rel_tol=1.22e-4)
    assert math.isclose(r_bar[2], 0.0, abs_tol=1.48e-8)
    assert math.isclose(q[0], 0.5 * math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(q[1], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(q[2], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(q[3], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
    assert math.isclose(v_bar[0], - G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
    assert math.isclose(v_bar[1], 7.0e3, rel_tol=1.22e-4)
    assert math.isclose(v_bar[2], 0.0, abs_tol=1.48e-8)
    assert math.isclose(om_bar[0], math.pi / 1.5 / math.sqrt(3.0))
    assert math.isclose(om_bar[1], math.pi / 1.5 / math.sqrt(3.0))
    assert math.isclose(om_bar[2], math.pi / 1.5 / math.sqrt(3.0))

