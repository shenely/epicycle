# built-in libraries
import math

# external libraries
import numpy

# internal libraries
from epicycle import interp


def test_interp_lerp():
    y = interp.lerp(0.0, 1.0, 2.0, 3.0, 0.5)
    assert y == 1.5


def test_interp_vlerp():
    v0_bar = numpy.array([1.0, -2.0, 3.0])
    v1_bar = numpy.array([4.0, -5.0, 6.0])
    v_bar = interp.vlerp(0.0, v0_bar, 2.0, v1_bar, 0.5)
    assert v_bar[0] == 1.75
    assert v_bar[1] == -2.75
    assert v_bar[2] == 3.75


def test_interp_slerp():
    q0 = numpy.array([1.0, 0.0, 0.0, 0.0])
    q1 = numpy.array([0.5, 0.5, 0.5, 0.5])
    q = interp.slerp(0.0, q0, 1.0, q1, 0.5)
    assert math.isclose(q[0], 0.5 * math.sqrt(3.0))
    assert math.isclose(q[1], 0.5 / math.sqrt(3.0))
    assert math.isclose(q[2], 0.5 / math.sqrt(3.0))
    assert math.isclose(q[3], 0.5 / math.sqrt(3.0))


def test_interp_cint():
    p0_bar = numpy.array([1.000, 0.000, 0.0])
    m0_bar = numpy.array([0.000, 0.524, 1.0])
    p1_bar = numpy.array([0.500, 0.866, 2.0])
    m1_bar = numpy.array([-0.453, 0.262, 1.0])
    p_bar, m_bar = interp.cint(0.0, p0_bar, m0_bar, 2.0, p1_bar, m1_bar, 1.0)
    assert math.isclose(p_bar[0], 0.866, rel_tol=5e-3)
    assert math.isclose(p_bar[1], 0.500, rel_tol=5e-3)
    assert math.isclose(p_bar[2], 1.0, rel_tol=5e-3)
    assert math.isclose(m_bar[0], -0.262, rel_tol=5e-3)
    assert math.isclose(m_bar[1], 0.453, rel_tol=5e-3)
    assert math.isclose(m_bar[2], 1.0, rel_tol=5e-3)


def test_interp_squad():
    q_a = numpy.array([1.0, 0.0, 0.0, 0.0])
    om_a = numpy.array([0.0, 0.0, 0.0])
    q_b = numpy.array([0.5, 0.5, 0.5, 0.5])
    om_b = numpy.array([0.0, 0.0, 0.0])
    q, om = interp.squad(0.0, q_a, om_a, 2.0, q_b, om_b, 1.0)
    assert math.isclose(q[0], 0.5 * math.sqrt(3.0))
    assert math.isclose(q[1], 0.5 / math.sqrt(3.0))
    assert math.isclose(q[2], 0.5 / math.sqrt(3.0))
    assert math.isclose(q[3], 0.5 / math.sqrt(3.0))
    assert math.isclose(om[0], 0.5 / math.sqrt(3.0) * math.pi)
    assert math.isclose(om[1], 0.5 / math.sqrt(3.0) * math.pi)
    assert math.isclose(om[2], 0.5 / math.sqrt(3.0) * math.pi)

