# built-in libraries
# ...

# external libraries
import numpy
import pytest

# internal libraries
from epicycle import dmat


def test_dmat_tr():
    A = numpy.array([1.0, -2.0, 3.0])
    s = dmat.tr(A)
    assert s == 2.0


def test_dmat_det():
    A = numpy.array([1.0, -2.0, 3.0])
    s = dmat.det(A)
    assert s == -6.0


def test_dmat_inv_good():
    A = numpy.array([1.0, -2.0, 4.0])
    A_inv = dmat.inv(A)
    assert A_inv[0] == -0.125
    assert A_inv[1] == 0.25
    assert A_inv[2] == -0.5


def test_dmat_inv_bad():
    A = numpy.array([0.0, -2.0, 4.0])
    with pytest.raises(ZeroDivisionError):
        dmat.inv(A)


def test_dmat_mul():
    A = numpy.array([1.0, -2.0, 3.0])
    B = numpy.array([-4.0, 5.0, -6.0])
    C = dmat.mul(A, B)
    assert C[0] == -4.0
    assert C[1] == -10.0
    assert C[2] == -18.0


def test_dmat_muld():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = numpy.array([1.0, -2.0, 3.0])
    C = dmat.muld(A, B)
    assert C[0][0] == 1.0
    assert C[0][1] == 4.0
    assert C[0][2] == 9.0
    assert C[1][0] == -4.0
    assert C[1][1] == -10.0
    assert C[1][2] == -18.0
    assert C[2][0] == 7.0
    assert C[2][1] == 16.0
    assert C[2][2] == 27.0


def test_dmat_dmul():
    A = numpy.array([1.0, -2.0, 3.0])
    B = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    C = dmat.dmul(A, B)
    assert C[0][0] == 1.0
    assert C[0][1] == -2.0
    assert C[0][2] == 3.0
    assert C[1][0] == 8.0
    assert C[1][1] == -10.0
    assert C[1][2] == 12.0
    assert C[2][0] == 21.0
    assert C[2][1] == -24.0
    assert C[2][2] == 27.0

