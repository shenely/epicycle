# built-in libraries
import math

# external libraries
import numpy.ctypeslib

# internal libraries
from epicycle.gee import G_MU
from epicycle.vehicle_model import vehicle_model_t
from epicycle._epicycle import EpicycleConsole


def test_epicycle():
    console = EpicycleConsole("/my.shm")
    try:
        console.open()
        with console:
            vehicle_model = vehicle_model_t.from_buffer(console)
            vehicle_model.size = 1
            vehicle_model.cfg.clk.delta_t = 1.0
            vehicle_model.cfg.obj_lst[0].q = numpy.ctypeslib.as_ctypes(
                numpy.array([1.0, 0.0, 0.0, 0.0])
            )
            vehicle_model.st.obj_lst[0].m = 1.0
            vehicle_model.st.obj_lst[0].I_cm = numpy.ctypeslib.as_ctypes(
                numpy.array([1.0 / 12.0, 1.0 / 12.0, 1.0 / 12.0])
            )
            vehicle_model.st.clk.t = 0.0
            vehicle_model.st.sys.r_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([7000.0e3, 0.0, 0.0])
            )
            vehicle_model.st.sys.q = numpy.ctypeslib.as_ctypes(
                numpy.array([1.0, 0.0, 0.0, 0.0])
            )
            vehicle_model.st.sys.v_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 7.0e3, 0.0])
            )
            vehicle_model.ch.clk.t = 1.0
            vehicle_model.in_.obj_lst[0].M_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([1.0, 1.0, 1.0]) * math.pi / 18.0 / math.sqrt(3.0)
            )
        with console:
            r_bar = numpy.ctypeslib.as_array(vehicle_model.st.sys.r_bar)
            q = numpy.ctypeslib.as_array(vehicle_model.st.sys.q)
            v_bar = numpy.ctypeslib.as_array(vehicle_model.st.sys.v_bar)
            om_bar = numpy.ctypeslib.as_array(vehicle_model.st.sys.om_bar)
            print(r_bar)
            print(q)
            print(v_bar)
            print(om_bar)
            assert math.isclose(r_bar[0], 7000.0e3 -0.5 * G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
            assert math.isclose(r_bar[1], 7.0e3, rel_tol=1.22e-4)
            assert math.isclose(r_bar[2], 0.0, abs_tol=1e-3)
            assert math.isclose(q[0], 0.5 * math.sqrt(3.0), rel_tol=1.22e-4)
            assert math.isclose(q[1], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
            assert math.isclose(q[2], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
            assert math.isclose(q[3], 0.5 / math.sqrt(3.0), rel_tol=1.22e-4)
            assert math.isclose(v_bar[0], - G_MU / 7000.0e3 ** 2, rel_tol=1.22e-4)
            assert math.isclose(v_bar[1], 7.0e3, rel_tol=1.22e-4)
            assert math.isclose(v_bar[2], 0.0, abs_tol=1e-3)
            assert math.isclose(om_bar[0], math.pi / 1.5 / math.sqrt(3.0))
            assert math.isclose(om_bar[0], math.pi / 1.5 / math.sqrt(3.0))
            assert math.isclose(om_bar[0], math.pi / 1.5 / math.sqrt(3.0))
    finally:
        console.close()

