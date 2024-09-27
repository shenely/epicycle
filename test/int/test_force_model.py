# built-in libraries
import math

# external libraries
import numpy.ctypeslib

# internal libraries
from epicycle import vec
from epicycle import quat
from epicycle.data_model import *
from epicycle.force_model import *


def test_interp_st():
    prev = st_t(
        clk=st_t.clk_t(t=0.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([1.000, 0.000, 0.0])
            ),
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.000, 0.524, 1.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 0.0, 0.0])
                ),
            ),
        ),
    )
    next = st_t(
        clk=st_t.clk_t(t=2.0),
        sys=st_t.sys_t(
            r_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.500, 0.866, 2.0])
            ),
            q=numpy.ctypeslib.as_ctypes(
                numpy.array([0.5, 0.5, 0.5, 0.5])
            ),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([-0.453, 0.262, 1.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
        ),
        obj_lst=(
            st_t.obj_t(),
        ),
    )
    curr = st_t(
        clk=st_t.clk_t(t=1.0),
    )
    interp_st(1, prev, next, curr)
    r_bar = numpy.ctypeslib.as_array(curr.sys.r_bar)
    q = numpy.ctypeslib.as_array(curr.sys.q)
    v_bar = numpy.ctypeslib.as_array(curr.sys.v_bar)
    om_bar = numpy.ctypeslib.as_array(curr.sys.om_bar)
    m = curr.obj_lst[0].m
    I_cm = numpy.ctypeslib.as_array(curr.obj_lst[0].I_cm)
    p_bar = numpy.ctypeslib.as_array(curr.obj_lst[0].p_bar)
    assert math.isclose(r_bar[0], 0.866, rel_tol=5e-3)
    assert math.isclose(r_bar[1], 0.500, rel_tol=5e-3)
    assert math.isclose(r_bar[2], 1.0, rel_tol=5e-3)
    assert math.isclose(q[0], 0.5 * math.sqrt(3.0))
    assert math.isclose(q[1], 0.5 / math.sqrt(3.0))
    assert math.isclose(q[2], 0.5 / math.sqrt(3.0))
    assert math.isclose(q[3], 0.5 / math.sqrt(3.0))
    assert math.isclose(v_bar[0], -0.262, rel_tol=5e-3)
    assert math.isclose(v_bar[1], 0.453, rel_tol=5e-3)
    assert math.isclose(v_bar[2], 1.0, rel_tol=5e-3)
    assert math.isclose(om_bar[0], 0.5 / math.sqrt(3.0) * math.pi)
    assert math.isclose(om_bar[1], 0.5 / math.sqrt(3.0) * math.pi)
    assert math.isclose(om_bar[2], 0.5 / math.sqrt(3.0) * math.pi)
    assert m == 0.5
    assert I_cm[0] == 0.5
    assert I_cm[1] == 0.0
    assert I_cm[2] == 0.0


def test_solve_ch_st():
    cfg = cfg_t()
    ch = ch_t(
        obj_lst=(
            ch_t.obj_t(
                T=ch_t.obj_t._T.E_ST.value,
                st=ch_t.obj_t._U.st_t(
                    m=1.0,
                    p_bar=numpy.ctypeslib.as_ctypes(
                        numpy.array([1.0, 0.0, 0.0])
                    ),
                    h_bar=numpy.ctypeslib.as_ctypes(
                        numpy.array([0.0, 0.0, 1.0])
                    ),
                ),
            ),
        ),
    )
    prev = st_t(
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 1.0, 0.0])
                ),
                p_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 0.0, 0.0])
                ),
                h_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 0.0, 1.0])
                ),
            ),
        ),
    )
    next = st_t()
    assert solve_ch(1, cfg, ch, prev, next, None, None)
    m = next.obj_lst[0].m
    I_cm = numpy.ctypeslib.as_array(next.obj_lst[0].I_cm)
    p_bar = numpy.ctypeslib.as_array(next.obj_lst[0].p_bar)
    h_bar = numpy.ctypeslib.as_array(next.obj_lst[0].h_bar)
    assert m == 2.0
    assert I_cm[0] == 0.0
    assert I_cm[1] == 2.0
    assert I_cm[2] == 0.0
    assert p_bar[0] == 2.0
    assert p_bar[1] == 0.0
    assert p_bar[2] == 0.0
    assert h_bar[0] == 0.0
    assert h_bar[1] == 0.0
    assert h_bar[2] == 2.0


def test_solve_ch_in():
    cfg = cfg_t()
    ch = ch_t(
        obj_lst=(
            ch_t.obj_t(
                T=ch_t.obj_t._T.E_IN.value,
                in_=ch_t.obj_t._U.in_t(
                    m_dot=1.0,
                    F_bar=numpy.ctypeslib.as_ctypes(
                        numpy.array([1.0, 0.0, 0.0])
                    ),
                    M_bar=numpy.ctypeslib.as_ctypes(
                        numpy.array([0.0, 0.0, 1.0])
                    ),
                ),
            ),
        ),
    )
    in_ = in_t()
    assert solve_ch(1, cfg, ch, None, None, in_, None)
    m_dot = in_.obj_lst[0].m_dot
    F_bar = numpy.ctypeslib.as_array(in_.obj_lst[0].F_bar)
    M_bar = numpy.ctypeslib.as_array(in_.obj_lst[0].M_bar)
    assert m_dot == 1.0
    assert F_bar[0] == 1.0
    assert F_bar[1] == 0.0
    assert F_bar[2] == 0.0
    assert M_bar[0] == 0.0
    assert M_bar[1] == 0.0
    assert M_bar[2] == 1.0


def test_solve_ch_em():
    cfg = cfg_t()
    ch = ch_t(
        obj_lst=(
            ch_t.obj_t(
                T=ch_t.obj_t._T.E_EM.value,
                em=ch_t.obj_t._U.em_t(
                    q=1.0,
                    p_bar=numpy.ctypeslib.as_ctypes(
                        numpy.array([1.0, 0.0, 0.0])
                    ),
                    m_bar=numpy.ctypeslib.as_ctypes(
                        numpy.array([0.0, 0.0, 1.0])
                    ),
                ),
            ),
        ),
    )
    em = em_t()
    assert solve_ch(1, cfg, ch, None, None, None, em)
    q = em.obj_lst[0].q
    p_bar = numpy.ctypeslib.as_array(em.obj_lst[0].p_bar)
    m_bar = numpy.ctypeslib.as_array(em.obj_lst[0].m_bar)
    assert q == 1.0
    assert p_bar[0] == 1.0
    assert p_bar[1] == 0.0
    assert p_bar[2] == 0.0
    assert m_bar[0] == 0.0
    assert m_bar[1] == 0.0
    assert m_bar[2] == 1.0


def test_solve_st_dot():
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
            om_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([1.0, 1.0, 1.0]) * math.pi / 1.5 / math.sqrt(3.0)
            ),
        ),
    )
    next = st_t()
    in_ = in_t()
    solve_st_dot(1, cfg, prev, next, in_)
    r_bar = numpy.ctypeslib.as_array(next.sys.r_bar)
    v_bar = numpy.ctypeslib.as_array(next.sys.v_bar)
    q = numpy.ctypeslib.as_array(next.sys.q)
    om_bar = numpy.ctypeslib.as_array(next.sys.om_bar)
    assert next.clk.t == 1.0
    assert r_bar[0] == 7000.0e3
    assert r_bar[1] == 7.0e3
    assert r_bar[2] == 0.0
    assert v_bar[0] == 0.0
    assert v_bar[1] == 7.0e3
    assert v_bar[2] == 0.0
    assert math.isclose(q[0], 0.5)
    assert math.isclose(q[1], 0.5)
    assert math.isclose(q[2], 0.5)
    assert math.isclose(q[3], 0.5)
    assert om_bar[0] == math.pi / 1.5 / math.sqrt(3.0)
    assert om_bar[1] == math.pi / 1.5 / math.sqrt(3.0)
    assert om_bar[2] == math.pi / 1.5 / math.sqrt(3.0)


def test_solve_st_delta():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(numpy.zeros((3))),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    prev = st_t(
        sys=st_t.sys_t(
            q=numpy.ctypeslib.as_ctypes(quat.one()),
            v_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([1.0, 0.0, 0.0])
            ),
            om_bar=numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 1.0])
            ),
        ),
        obj_lst=(
            st_t.obj_t(
                m=2.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0])
                ),
                p_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([2.0, 0.0, 0.0])
                ),
                h_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 0.0, 1.0 / 6.0])
                ),
            ),
        ),
    )
    next = st_t(
        sys=st_t.sys_t(
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
                p_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 0.0, 0.0])
                ),
                h_bar=numpy.ctypeslib.as_ctypes(

                    numpy.array([0.0, 0.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    out = out_t()
    solve_st_delta(1, cfg, prev, next, out)
    v_bar = numpy.ctypeslib.as_array(next.sys.v_bar)
    om_bar = numpy.ctypeslib.as_array(next.sys.om_bar)
    assert v_bar[0] == 3.0
    assert v_bar[1] == 0.0
    assert v_bar[2] == 0.0
    assert om_bar[0] == 0.0
    assert om_bar[1] == 0.0
    assert om_bar[2] == 3.0


def test_solve_in():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([
                        math.cos(math.radians(-30.0)),
                        math.sin(math.radians(-30.0)),
                        0.0
                    ])
                ),
                q=numpy.ctypeslib.as_ctypes(
                    numpy.array([
                        math.cos(math.radians(-60.0 / 2)),
                        0.0,
                        0.0,
                        math.sin(math.radians(-60.0 / 2))
                    ])
                ),
            ),
        ),
    )
    st = st_t(
        sys=st_t.sys_t(
            q=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    math.cos(math.radians(30.0 / 2)),
                    0.0,
                    0.0,
                    math.sin(math.radians(30.0 / 2))
                ])
            ),
        ),
    )
    in_ = in_t(
        obj_lst=(
            in_t.obj_t(
                F_bar=numpy.ctypeslib.as_ctypes(numpy.array([1.0, 0.0, 0.0])),
                M_bar=numpy.ctypeslib.as_ctypes(numpy.array([0.0, 0.0, 1.0])),
            ),
        ),
    )
    out = out_t(
        sys=out_t.sys_t(
            m=1.0,
            I_cm=numpy.ctypeslib.as_ctypes(
                numpy.array([
                    [1.0 / 12.0, 0.0, 0.0],
                    [0.0, 1.0 / 12.0, 0.0],
                    [0.0, 0.0, 1.0 / 12.0],
                ])
            ),
        )
    )
    solve_in(1, cfg, st, in_, out)
    F_bar = numpy.ctypeslib.as_array(in_.sys.F_bar)
    M_bar = numpy.ctypeslib.as_array(in_.sys.M_bar)
    v_dot = numpy.ctypeslib.as_array(in_.sys.v_dot)
    om_dot = numpy.ctypeslib.as_array(in_.sys.om_dot)
    assert math.isclose(F_bar[0], 0.5 * math.sqrt(3.0))
    assert math.isclose(F_bar[1], -0.5)
    assert F_bar[2] == 0.0
    assert M_bar[0] == 0.0
    assert M_bar[1] == 0.0
    assert M_bar[2] == 0.5
    assert math.isclose(v_dot[0], 0.5 * math.sqrt(3.0))
    assert math.isclose(v_dot[1], -0.5)
    assert v_dot[2] == 0.0
    assert om_dot[0] == 0.0
    assert om_dot[1] == 0.0
    assert om_dot[2] == 6.0


def test_solve_out():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 0.0, 0.0])
                ),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 1.0, 0.0])
                ),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    st = st_t(
        sys=st_t.sys_t(
            q=numpy.ctypeslib.as_ctypes(quat.one()),
        ),
        obj_lst=(
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
            st_t.obj_t(
                m=1.0,
                I_cm=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
                ),
            ),
        ),
    )
    out = out_t()
    solve_out(2, cfg, st, out)
    m = out.sys.m
    c_bar = numpy.ctypeslib.as_array(out.sys.c_bar)
    I_cm = numpy.ctypeslib.as_array(out.sys.I_cm)
    assert m == 2.0
    assert c_bar[0] == 0.5
    assert c_bar[1] == 0.5
    assert c_bar[2] == 0.0
    assert math.isclose(I_cm[0,0], 2.0 / 3.0)
    assert I_cm[0,1] == 0.5
    assert I_cm[0,2] == 0.0
    assert I_cm[1,0] == 0.5
    assert math.isclose(I_cm[1,1], 2.0 / 3.0)
    assert I_cm[1,2] == 0.0
    assert I_cm[2,0] == 0.0
    assert I_cm[2,1] == 0.0
    assert math.isclose(I_cm[2,2], 7.0 / 6.0)


def test_solve_em():
    cfg = cfg_t(
        obj_lst=(
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([1.0, 0.0, 0.0])
                ),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
            cfg_t.obj_t(
                r_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 1.0, 0.0])
                ),
                q=numpy.ctypeslib.as_ctypes(quat.one()),
            ),
        ),
    )
    em = em_t(
        obj_lst=(
            em_t.obj_t(
                q=1.0,
                p_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 0.0, 1.0])
                ),
            ),
            em_t.obj_t(
                q=-1.0,
                m_bar=numpy.ctypeslib.as_ctypes(
                    numpy.array([0.0, 0.0, 1.0])
                ),
            ),
        ),
    )
    solve_em(2, cfg, em)
    q = em.sys.q
    p_bar = numpy.ctypeslib.as_array(em.sys.p_bar)
    m_bar = numpy.ctypeslib.as_array(em.sys.m_bar)
    assert q == 0.0
    assert p_bar[0] == 1.0
    assert p_bar[1] == -1.0
    assert p_bar[2] == 1.0
    assert m_bar[0] == 0.0
    assert m_bar[1] == 0.0
    assert m_bar[2] == 1.0

