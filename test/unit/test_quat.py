# built-in libraries
import math

# external libraries
import numpy
import pytest

# internal libraries
from epicycle import quat


def test_quat_one():
    q = quat.one()
    assert q[0] == 1.0
    assert q[1] == 0.0
    assert q[2] == 0.0
    assert q[3] == 0.0


def test_quat_neg():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = quat.neg(p)
    assert q[0] == -2.0
    assert q[1] == 3.0
    assert q[2] == -4.0
    assert q[3] == 5.0


def test_quat_conj():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = quat.conj(p)
    assert q[0] == 2.0
    assert q[1] == 3.0
    assert q[2] == -4.0
    assert q[3] == 5.0


def test_quat_norm():
    q = numpy.array([2.0, -3.0, 4.0, -5.0])
    s = quat.norm(q)
    assert s == math.sqrt(54.0)


def test_quat_inv_good():
    q = numpy.array([2.0, -3.0, 4.0, -5.0])
    q_inv = quat.inv(q)
    assert q_inv[0] == 2.0 / 54.0
    assert q_inv[1] == 3.0 / 54.0
    assert q_inv[2] == -4.0 / 54.0
    assert q_inv[3] == 5.0 / 54.0


def test_quat_inv_bad():
    q = numpy.zeros((4,))
    with pytest.raises(ZeroDivisionError):
        quat.inv(q)


def test_quat_unit_good():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = quat.unit(p)
    assert q[0] == 2.0 / math.sqrt(54.0)
    assert q[1] == -3.0 / math.sqrt(54.0)
    assert q[2] == 4.0 / math.sqrt(54.0)
    assert q[3] == -5.0 / math.sqrt(54.0)


def test_quat_unit_bad():
    q = numpy.zeros((4,))
    with pytest.raises(ZeroDivisionError):
        quat.unit(q)


def test_quat_add():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = numpy.array([-6.0, 7.0, -8.0, 9.0])
    r = quat.add(p, q)
    assert r[0] == -4.0
    assert r[1] == 4.0
    assert r[2] == -4.0
    assert r[3] == 4.0


def test_quat_sub():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = numpy.array([-6.0, 7.0, -8.0, 9.0])
    r = quat.sub(p, q)
    assert r[0] == 8.0
    assert r[1] == -10.0
    assert r[2] == 12.0
    assert r[3] == -14.0


def test_quat_muls():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = quat.muls(p, 3.0)
    assert q[0] == 6.0
    assert q[1] == -9.0
    assert q[2] == 12.0
    assert q[3] == -15.0


def test_quat_mulv():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    v_bar = numpy.array([7.0, -8.0, 9.0])
    q = quat.mulv(p, v_bar)
    assert q[0] == 98.0
    assert q[1] == 10.0
    assert q[2] == -24.0
    assert q[3] == 14.0


def test_quat_vmul():
    v_bar = numpy.array([-3.0, 4.0, -5.0])
    p = numpy.array([-6.0, 7.0, -8.0, 9.0])
    q = quat.vmul(v_bar, p)
    assert q[0] == 98.0
    assert q[1] == 14.0
    assert q[2] == -32.0
    assert q[3] == 26.0


def test_quat_mul():
    p = numpy.array([2.0, -3.0, 4.0, -5.0])
    q = numpy.array([-6.0, 7.0, -8.0, 9.0])
    r = quat.mul(p, q)
    assert r[0] == 86.0
    assert r[1] == 28.0
    assert r[2] == -48.0
    assert r[3] == 44.0


def test_quat_pow():
    p = numpy.array([0.5, 0.5, 0.5, 0.5])
    q = quat.pow(p, 2.0)
    assert math.isclose(q[0], -0.5)
    assert math.isclose(q[1], 0.5)
    assert math.isclose(q[2], 0.5)
    assert math.isclose(q[3], 0.5)


def test_quat_log():
    q = numpy.array([0.5, 0.5, 0.5, 0.5])
    v_bar = quat.log(q)
    assert math.isclose(v_bar[0], math.pi / math.sqrt(27))
    assert math.isclose(v_bar[1], math.pi / math.sqrt(27))
    assert math.isclose(v_bar[2], math.pi / math.sqrt(27))


def test_quat_rot_mat():
    q = numpy.array([0.5, 0.5, 0.5, 0.5])
    R = quat.rot_mat(q)
    assert R[0][0] == 0.0
    assert R[0][1] == 0.0
    assert R[0][2] == 1.0
    assert R[1][0] == 1.0
    assert R[1][1] == 0.0
    assert R[1][2] == 0.0
    assert R[2][0] == 0.0
    assert R[2][1] == 1.0
    assert R[2][2] == 0.0


def test_quat_irot_mat():
    q = numpy.array([0.5, 0.5, 0.5, 0.5])
    R = quat.irot_mat(q)
    assert R[0][0] == 0.0
    assert R[0][1] == 1.0
    assert R[0][2] == 0.0
    assert R[1][0] == 0.0
    assert R[1][1] == 0.0
    assert R[1][2] == 1.0
    assert R[2][0] == 1.0
    assert R[2][1] == 0.0
    assert R[2][2] == 0.0

