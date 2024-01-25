# built-in libraries
# ...

# external libraries
# ...

# internal libraries
from epicycle import poly
from epicycle import poly_t


def test_poly_zero():
    P = poly.zero()
    for i in range(P.deg + 1):
        assert P.coeff[i] == 0.0


def test_poly_pos():
    P = poly_t(2, (4.0, -5.0, 3.0))
    Q = poly.pos(P)
    for i in range(3):
        assert Q.coeff[i] == P.coeff[i]
    assert Q.deg == P.deg


def test_poly_neg():
    P = poly_t(2, (4.0, -5.0, 3.0))
    Q = poly.neg(P)
    for i in range(3):
        assert Q.coeff[i] == -P.coeff[i]
    assert Q.deg == P.deg


def test_poly_add():
    P = poly_t(2, (5.0, 6.0, 2.0))
    Q = poly_t(2, (-1.0, -2.0, 3.0))
    R = poly.add(P, Q)
    assert R.deg == max(P.deg, Q.deg)
    assert R.coeff[0] == 4.0
    assert R.coeff[1] == 4.0
    assert R.coeff[2] == 5.0


def test_poly_sub():
    P = poly_t(2, (4.0, 4.0, 5.0))
    Q = poly_t(2, (-1.0, -2.0, 3.0))
    R = poly.sub(P, Q)
    assert R.deg == max(P.deg, Q.deg)
    assert R.coeff[0] == 5.0
    assert R.coeff[1] == 6.0
    assert R.coeff[2] == 2.0


def test_poly_mul():
    P = poly_t(2, (-8.0, 2.0, 3.0))
    Q = poly_t(1, (5.0, 1.0))
    R = poly.mul(P, Q)
    assert R.deg == P.deg + Q.deg
    assert R.coeff[0] == -40.0
    assert R.coeff[1] == 2.0
    assert R.coeff[2] == 17.0
    assert R.coeff[3] == 3.0


def test_poly_div():
    N = poly_t(2, (-1.0, -5.0, 2.0))
    D = poly_t(1, (-3.0, 1.0))
    Q, R = poly.div(N, D)
    assert Q.deg == 1
    assert Q.coeff[0] == 1.0
    assert Q.coeff[1] == 2.0
    assert R.deg == 0
    assert R.coeff[0] == 2.0


def test_poly_diff():
    P = poly_t(3, (-7.0, 0.0, 2.0, 4.0))
    diff_P = poly.diff(P)
    assert diff_P.deg == 2
    assert diff_P.coeff[0] == 0.0
    assert diff_P.coeff[1] == 4.0
    assert diff_P.coeff[2] == 12.0


def test_poly_int():
    P = poly_t(3, (-7.0, 0.0, 2.0, 4.0))
    int_P = poly.int(P)
    assert int_P.deg == 4
    assert int_P.coeff[0] == 0.0
    assert int_P.coeff[1] == -7.0 / 1.0
    assert int_P.coeff[2] == 0.0 / 2.0
    assert int_P.coeff[3] == 2.0 / 3.0
    assert int_P.coeff[4] == 4.0 / 4.0


def test_poly_eval():
    P = poly_t(3, (-7.0, 0.0, 2.0, 4.0))
    s = poly.eval(P, 2.0)
    assert s == 33.0

