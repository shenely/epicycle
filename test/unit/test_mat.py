# built-in libraries
# ...

# external libraries
import numpy
import pytest

# internal libraries
from epicycle import mat


def test_mat_zero():
    O = mat.zero()
    assert O[0][0] == 0.0
    assert O[0][1] == 0.0
    assert O[0][2] == 0.0
    assert O[1][0] == 0.0
    assert O[1][1] == 0.0
    assert O[1][2] == 0.0
    assert O[2][0] == 0.0
    assert O[2][1] == 0.0
    assert O[2][2] == 0.0


def test_mat_eye():
    I = mat.eye()
    assert I[0][0] == 1.0
    assert I[0][1] == 0.0
    assert I[0][2] == 0.0
    assert I[1][0] == 0.0
    assert I[1][1] == 1.0
    assert I[1][2] == 0.0
    assert I[2][0] == 0.0
    assert I[2][1] == 0.0
    assert I[2][2] == 1.0


def test_mat_pos():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = mat.pos(A)
    assert B[0][0] == 1.0
    assert B[0][1] == -2.0
    assert B[0][2] == 3.0
    assert B[1][0] == -4.0
    assert B[1][1] == 5.0
    assert B[1][2] == -6.0
    assert B[2][0] == 7.0
    assert B[2][1] == -8.0
    assert B[2][2] == 9.0


def test_mat_neg():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = mat.neg(A)
    assert B[0][0] == -1.0
    assert B[0][1] == 2.0
    assert B[0][2] == -3.0
    assert B[1][0] == 4.0
    assert B[1][1] == -5.0
    assert B[1][2] == 6.0
    assert B[2][0] == -7.0
    assert B[2][1] == 8.0
    assert B[2][2] == -9.0


def test_mat_tr():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    s = mat.tr(A)
    assert s == 15.0


def test_mat_det():
    A = numpy.array([[6.0, 1.0, 1.0], [4.0, -2.0, 5.0], [2.0, 8.0, 7.0]])
    s = mat.det(A)
    assert s == -306.0


def test_mat_inv_good():
    A = numpy.array([[3.0, 0.0, 2.0], [2.0, 0.0, -2.0], [0.0, 1.0, 1.0]])
    A_inv = mat.inv(A)
    assert A_inv[0][0] == 0.2
    assert A_inv[0][1] == 0.2
    assert A_inv[0][2] == 0.0
    assert A_inv[1][0] == -0.2
    assert A_inv[1][1] == 0.3
    assert A_inv[1][2] == 1.0
    assert A_inv[2][0] == 0.2
    assert A_inv[2][1] == -0.3
    assert A_inv[2][2] == 0.0


def test_mat_inv_bad():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    with pytest.raises(ZeroDivisionError):
        mat.inv(A)


def test_mat___T():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    A__T = mat.T(A)
    print(A__T)
    assert A__T[0][0] == 1.0
    assert A__T[0][1] == -4.0
    assert A__T[0][2] == 7.0
    assert A__T[1][0] == -2.0
    assert A__T[1][1] == 5.0
    assert A__T[1][2] == -8.0
    assert A__T[2][0] == 3.0
    assert A__T[2][1] == -6.0
    assert A__T[2][2] == 9.0


def test_mat_add():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = numpy.array([[1.0, -4.0, 7.0], [-2.0, 5.0, -8.0], [3.0, -6.0, 9.0]])
    C = mat.add(A, B)
    assert C[0][0] == 2.0
    assert C[0][1] == -6.0
    assert C[0][2] == 10.0
    assert C[1][0] == -6.0
    assert C[1][1] == 10.0
    assert C[1][2] == -14.0
    assert C[2][0] == 10.0
    assert C[2][1] == -14.0
    assert C[2][2] == 18.0


def test_mat_sub():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = numpy.array([[1.0, -4.0, 7.0], [-2.0, 5.0, -8.0], [3.0, -6.0, 9.0]])
    C = mat.sub(A, B)
    assert C[0][0] == 0.0
    assert C[0][1] == 2.0
    assert C[0][2] == -4.0
    assert C[1][0] == -2.0
    assert C[1][1] == 0.0
    assert C[1][2] == 2.0
    assert C[2][0] == 4.0
    assert C[2][1] == -2.0
    assert C[2][2] == 0.0


def test_mat_muls():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = mat.muls(A, 3.0)
    assert B[0][0] == 3.0
    assert B[0][1] == -6.0
    assert B[0][2] == 9.0
    assert B[1][0] == -12.0
    assert B[1][1] == 15.0
    assert B[1][2] == -18.0
    assert B[2][0] == 21.0
    assert B[2][1] == -24.0
    assert B[2][2] == 27.0


def test_mat_mul():
    A = numpy.array([[1.0, -2.0, 3.0], [-4.0, 5.0, -6.0], [7.0, -8.0, 9.0]])
    B = numpy.array([[1.0, -4.0, 7.0], [-2.0, 5.0, -8.0], [3.0, -6.0, 9.0]])
    C = mat.mul(A, B)
    assert C[0][0] == 14.0
    assert C[0][1] == -32.0
    assert C[0][2] == 50.0
    assert C[1][0] == -32.0
    assert C[1][1] == 77.0
    assert C[1][2] == -122.0
    assert C[2][0] == 50.0
    assert C[2][1] == -122.0
    assert C[2][2] == 194.0


def test_mat_mulv():
    A = numpy.array([[3.0, 2.0, -1.0], [2.0, -2.0, 4.0], [-1.0, 0.5, -1.0]])
    u_bar = numpy.array([1.0, -2.0, -2.0])
    v_bar = mat.mulv(A, u_bar)
    assert v_bar[0] == 1.0
    assert v_bar[1] == -2.0
    assert v_bar[2] == 0.0


def test_mat_vmul():
    u_bar = numpy.array([1.0, -2.0, -2.0])
    A = numpy.array([[3.0, 2.0, -1.0], [2.0, -2.0, 0.5], [-1.0, 4.0, -1.0]])
    v_bar = mat.vmul(u_bar, A)
    print(u_bar)
    print(A)
    print(v_bar)
    assert v_bar[0] == 1.0
    assert v_bar[1] == -2.0
    assert v_bar[2] == 0.0

