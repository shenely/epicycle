# built-in libraries
import math

# external libraries
import numpy
import pytest

# internal libraries
from epicycle import gvec


def test_gvec_neg():
    u_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_bar = gvec.neg(u_bar)
    assert v_bar[0] == -1.0
    assert v_bar[1] == 2.0
    assert v_bar[2] == -3.0
    assert v_bar[3] == 4.0
    assert v_bar[4] == -5.0
    assert v_bar[5] == 6.0
    assert v_bar[6] == -7.0
    assert v_bar[7] == 6.0
    assert v_bar[8] == -5.0
    assert v_bar[9] == 4.0
    assert v_bar[10] == -3.0
    assert v_bar[11] == 2.0
    assert v_bar[12] == -1.0


def test_gvec_norm():
    v_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    s = gvec.norm(v_bar)
    assert s == math.sqrt(231.0)


def test_gvec_unit_good():
    v_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_hat = gvec.unit(v_bar)
    assert math.isclose(v_hat[0], 1.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[1], -2.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[2], 3.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[3], -4.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[4], 5.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[5], -6.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[6], 7.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[7], -6.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[8], 5.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[9], -4.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[10], 3.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[11], -2.0 / math.sqrt(231.0))
    assert math.isclose(v_hat[12], 1.0 / math.sqrt(231.0))


def test_gvec_unit_bad():
    o_bar = numpy.zeros((gvec.GMAT_NDIM,))
    with pytest.raises(ZeroDivisionError):
        gvec.unit(o_bar)


def test_gvec_add():
    u_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_bar = numpy.array(
        [7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0]
    )
    w_bar = gvec.add(u_bar, v_bar)
    assert w_bar[0] == 8.0
    assert w_bar[1] == -8.0
    assert w_bar[2] == 8.0
    assert w_bar[3] == -8.0
    assert w_bar[4] == 8.0
    assert w_bar[5] == -8.0
    assert w_bar[6] == 8.0
    assert w_bar[7] == -8.0
    assert w_bar[8] == 8.0
    assert w_bar[9] == -8.0
    assert w_bar[10] == 8.0
    assert w_bar[11] == -8.0
    assert w_bar[12] == 8.0


def test_gvec_sub():
    u_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_bar = numpy.array(
        [7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0]
    )
    w_bar = gvec.sub(u_bar, v_bar)
    assert w_bar[0] == -6.0
    assert w_bar[1] == 4.0
    assert w_bar[2] == -2.0
    assert w_bar[3] == 0.0
    assert w_bar[4] == 2.0
    assert w_bar[5] == -4.0
    assert w_bar[6] == 6.0
    assert w_bar[7] == -4.0
    assert w_bar[8] == 2.0
    assert w_bar[9] == 0.0
    assert w_bar[10] == -2.0
    assert w_bar[11] == 4.0
    assert w_bar[12] == -6.0


def test_gvec_muls():
    u_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_bar = gvec.muls(u_bar, 3.0)
    assert v_bar[0] == 3.0
    assert v_bar[1] == -6.0
    assert v_bar[2] == 9.0
    assert v_bar[3] == -12.0
    assert v_bar[4] == 15.0
    assert v_bar[5] == -18.0
    assert v_bar[6] == 21.0
    assert v_bar[7] == -18.0
    assert v_bar[8] == 15.0
    assert v_bar[9] == -12.0
    assert v_bar[10] == 9.0
    assert v_bar[11] == -6.0
    assert v_bar[12] == 3.0


def test_gvec_dot():
    u_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_bar = numpy.array(
        [7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0]
    )
    s = gvec.dot(u_bar, v_bar)
    assert s == 161.0


def test_gvec_omul():
    u_bar = numpy.array(
        [1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0]
    )
    v_bar = numpy.array(
        [7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0]
    )
    A = gvec.omul(u_bar, v_bar)
    for i in range(gvec.GMAT_NDIM):
        for j in range(gvec.GMAT_NDIM):
            assert A[i][j] == u_bar[i] * v_bar[j]

