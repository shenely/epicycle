import math
import enum

import numpy
import scipy.interpolate
from matplotlib import pyplot

from epicycle import stdatm

r0 = 6356.776e3
g0 = 9.80665
T0 = 288.15
M0 = 28.9644e-3
R_star = 8.31432


M = scipy.interpolate.interp1d(
    numpy.array([
        80.0e3, 80.5e3, 81.0e3, 81.5e3,
        82.0e3, 82.5e3, 83.0e3, 83.5e3,
        84.0e3, 84.5e3, 85.0e3, 85.5e3,
        86.0e3,
    ]),
    numpy.array([
        1.0,      0.999996, 0.999989, 0.999971,
        0.999941, 0.999909, 0.999870, 0.999829,
        0.999786, 0.999741, 0.999694, 0.999641,
        0.999579,
    ]) * M0
)


def H(Z: float) -> float:
    return r0 * Z / (r0 + Z)
    

def T(z):
    if z < 86e3:
        h = H(z)
        th = T0
        th -= 6.5e-3 *  min(    h, 11e3)
        th += 1.0e-3 * (min(max(h, 20e3),   32e3 ) - 20e3)
        th += 2.8e-3 * (min(max(h, 32e3),   47e3 ) - 32e3)
        th -= 2.8e-3 * (min(max(h, 51e3),   71e3 ) - 51e3)
        th -= 2.0e-3 * (min(max(h, 71e3), H(86e3)) - 71e3)
        if z > 80e3:
            th *= M(z) / M0
    elif z < 91e3:
        th = 186.8673
    elif z < 110e3:
        th = 263.1905 - 76.3232 * math.sqrt(1 - ((z - 91e3) / -19.9429e3) ** 2)
    elif z < 120e3:
        th = 240.0 + 12e-3 * (z - 110e3)
    else:
        th = 1000.0 - 640.0 * math.exp(- 0.01875e-3 * (z - 120e3) * (r0 + 120e3) / (r0 + z))
    return th
    
    
def g(z: float) -> float:
    return g0 * (r0 / (r0 + z)) ** 2
    

def spline(x, y):
    h = numpy.empty_like(x)
    h[1:] = numpy.diff(x)
    a = numpy.empty_like(x)
    a[1:-1] = h[1:-1] / (h[1:-1] + h[2:])
    a[-1] = 1
    b = 2 * numpy.ones_like(x)
    c = numpy.empty_like(x)
    c[0] = 1
    c[1:-1] = h[2:] / (h[1:-1] + h[2:])
    d = numpy.empty_like(x)
    d[0] = (6 / h[1]) * (
        (y[1] - y[0]) / h[1]
        + g(x[0]) * M(x[0]) / R_star / 186.8673
    )
    d[1:-1] = 6 * (
        (y[2:] - y[1:-1]) / h[2:]
        - (y[1:-1] - y[:-2]) / h[1:-1]
    ) / (x[2:] - x[:-2])
    d[-1] = (6 / h[-1]) * (
        0
        - (y[-1] - y[-2]) / h[-1]
    )
    for i in range(1, len(x)):
        b[i] -= a[i] / b[i-1] * c[i-1]
        d[i] -= a[i] / b[i-1] * d[i-1]
    k = numpy.empty_like(x)
    k[-1] = d[-1] / b[-1]
    for i in range(len(x)-2, -1, -1):
        k[i] = (d[i] - c[i] * k[i+1]) / b[i]
    print(k)
    return [
        numpy.poly1d([
            (k[i] - k[i-1]) / h[i] / 6,
            - 3 * (k[i] * x[i-1] - k[i-1] * x[i]) / h[i] / 6,
            (
                3 * (k[i] * x[i-1] ** 2 - k[i-1] * x[i] ** 2)
                - h[i] ** 2 * (k[i] - k[i-1])
                + 6 * (y[i] - y[i-1])
            ) / h[i] / 6,
            - (
                (k[i] * x[i-1] ** 3 - k[i-1] * x[i] ** 3)
                - h[i] ** 2 * (k[i] * x[i-1] - k[i-1] * x[i])
                + 6 * (y[i] * x[i-1] - y[i-1] * x[i])
            ) / h[i] / 6,
        ])
        for i in range(1, len(h))
    ], x
    

def uasa():
    z = numpy.array([
        86e3, 100e3, 115e3, 130e3, 150e3, 175e3,
        200e3, 250e3, 300e3, 400e3, 500e3, 600e3,
        700e3, 800e3, 900e3, 1000e3,
    ])
    p = numpy.array([
        3.7338e-1, 3.2011e-2, 4.0096e-3, 1.2505e-3, 4.5422e-4, 1.7936e-4,
        8.4736e-5, 2.4767e-5, 8.7704e-6, 1.4518e-6, 3.0236e-7, 8.2130e-8,
        3.1908e-8, 1.7036e-8, 1.0873e-8, 7.5138e-9,
    ])
    rho = numpy.array([
        6.958e-6, 5.604e-7, 4.289e-8, 8.152e-9, 2.076e-9, 6.339e-10,
        2.541e-10, 6.073e-11, 1.916e-11, 2.803e-12, 5.215e-13, 1.137e-13,
        3.070e-14, 1.136e-14, 5.759e-15, 3.561e-15,
    ])
    return spline(z, numpy.log(p)), spline(z, numpy.log(rho)), 


def main():
    uasa()
    fig, ax1 = pyplot.subplots()
    ax2 = ax1.twiny()
    z = numpy.linspace(0, 1000e3, num=1001)
    th = numpy.empty_like(z)
    p = numpy.empty_like(z)
    rho = numpy.empty_like(z)
    pt = stdatm.atm_t()
    for i in range(len(z)):
        pt = stdatm.eval(z[i])
        th[i] = pt.th
        p[i] = pt.p
        rho[i] = pt.rho
    else:
        ax1.plot(th, z / 1e3)
        ax2.semilogx(p, z / 1e3)
        ax2.semilogx(rho, z / 1e3)
        pyplot.show()


if __name__ == "__main__":
    main()

