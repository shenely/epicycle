# built-in libraries
import math

# external libraries
import numpy
import pytest

# internal libraries
from epicycle import vec


def test_vec_neg():
    u_bar = numpy.array([3.0, -4.0, 5.0])
    v_bar = vec.neg(u_bar)
    assert v_bar[0] == -3.0
    assert v_bar[1] == 4.0
    assert v_bar[2] == -5.0


def test_vec_norm():
    v_bar = numpy.array([1.0, -2.0, 3.0])
    s = vec.norm(v_bar)
    assert s == math.sqrt(14.0)


def test_vec_unit_good():
    v_bar = numpy.array([1.0, -2.0, 3.0])
    v_hat = vec.unit(v_bar)
    assert math.isclose(v_hat[0], 1.0 / math.sqrt(14.0))
    assert math.isclose(v_hat[1], -2.0 / math.sqrt(14.0))
    assert math.isclose(v_hat[2], 3.0 / math.sqrt(14.0))


def test_vec_unit_bad():
    o_bar = numpy.zeros((3,))
    with pytest.raises(ZeroDivisionError):
        vec.unit(o_bar)


def test_vec_add():
    u_bar = numpy.array([3.0, 7.0, 4.0])
    v_bar = numpy.array([2.0, 9.0, 11.0])
    w_bar = vec.add(u_bar, v_bar)
    assert w_bar[0] == 5.0
    assert w_bar[1] == 16.0
    assert w_bar[2] == 15.0


def test_vec_sub():
    u_bar = numpy.array([5.0, 16.0, 15.0])
    v_bar = numpy.array([2.0, 9.0, 11.0])
    w_bar = vec.sub(u_bar, v_bar)
    assert w_bar[0] == 3.0
    assert w_bar[1] == 7.0
    assert w_bar[2] == 4.0


def test_vec_muls():
    u_bar = numpy.array([7.0, 3.0, 0.0])
    v_bar = vec.muls(u_bar, 3.0)
    assert v_bar[0] == 21.0
    assert v_bar[1] == 9.0
    assert v_bar[2] == 0.0


def test_vec_dot():
    u_bar = numpy.array([4.0, 8.0, 10.0])
    v_bar = numpy.array([9.0, 2.0, 7.0])
    s = vec.dot(u_bar, v_bar)
    assert s == 122.0


def test_vec_cross():
    u_bar = numpy.array([2.0, 3.0, 4.0])
    v_bar = numpy.array([5.0, 6.0, 7.0])
    w_bar = vec.cross(u_bar, v_bar)
    assert w_bar[0] == -3.0
    assert w_bar[1] == 6.0
    assert w_bar[2] == -3.0


def test_vec_omul():
    u_bar = numpy.array([2.0, 3.0, 4.0])
    v_bar = numpy.array([5.0, 6.0, 7.0])
    A = vec.omul(u_bar, v_bar)
    assert A[0][0] == 10.0
    assert A[0][1] == 12.0
    assert A[0][2] == 14.0
    assert A[1][0] == 15.0
    assert A[1][1] == 18.0
    assert A[1][2] == 21.0
    assert A[2][0] == 20.0
    assert A[2][1] == 24.0
    assert A[2][2] == 28.0


def test_vec_cross_mat():
    u_bar = numpy.array([2.0, 3.0, 4.0])
    A = vec.cross_mat(u_bar)
    assert A[0][0] == 0.0
    assert A[0][1] == -4.0
    assert A[0][2] == 3.0
    assert A[1][0] == 4.0
    assert A[1][1] == 0.0
    assert A[1][2] == -2.0
    assert A[2][0] == -3.0
    assert A[2][1] == 2.0
    assert A[2][2] == 0.0


def test_vec_exp():
    v_bar = numpy.array([1.0, 1.0, 1.0]) *  math.pi / math.sqrt(27)
    q = vec.exp(v_bar)
    assert math.isclose(q[0], 0.5)
    assert math.isclose(q[1], 0.5)
    assert math.isclose(q[2], 0.5)
    assert math.isclose(q[3], 0.5)


def test_vec_rot():
    q = numpy.array([0.5, 0.5, 0.5, 0.5])
    u_bar = numpy.array([2.0, 3.0, 4.0])
    v_bar = vec.rot(q, u_bar)
    assert v_bar[0] == 4.0
    assert v_bar[1] == 2.0
    assert v_bar[2] == 3.0


def test_vec_irot():
    q = numpy.array([0.5, 0.5, 0.5, 0.5])
    u_bar = numpy.array([2.0, 3.0, 4.0])
    v_bar = vec.irot(q, u_bar)
    assert v_bar[0] == 3.0
    assert v_bar[1] == 4.0
    assert v_bar[2] == 2.0

