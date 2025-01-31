import math

import numpy
import scipy.linalg
from matplotlib import pyplot
import mpl_toolkits.mplot3d.art3d
import matplotlib.animation

from epicycle.vehicle_model import vehicle_model_t, ch_t, obj_t
from epicycle._epicycle import EpicycleConsole

i_hat = numpy.array([1.0, 0.0, 0.0])
j_hat = numpy.array([0.0, 1.0, 0.0])
k_hat = numpy.array([0.0, 0.0, 1.0])


def setup(console):
    with console:
        vehicle_model = vehicle_model_t.from_buffer(console)
        vehicle_model.size = 10
        vehicle_model.cfg.clk.delta_t = 1.0
        
        # bus
        bus = obj_t(vehicle_model, 0)
        bus.cfg.sym = "BUS".encode()
        bus.cfg.bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([5e-2, 5e-2, 5e-2])
        )
        bus.cfg.r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.0, 0.0, 0.0])
        )
        bus.cfg.q = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 0.0, 0.0, 0.0])
        )
        bus.st.m = 360e-3
        bus.st.I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([8e-4, 8e-4, 8e-4])
        )
        
        # rw A
        rw_a = obj_t(vehicle_model, 1)
        rw_a.cfg.sym = "RW_A".encode()
        rw_a.cfg.bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([25e-2, 25e-2, 15e-2]) / 2
        )
        rw_a.cfg.r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.2, 0.2, 0.2])
        )
        rw_a.cfg.q = numpy.ctypeslib.as_ctypes(
            numpy.array([
                math.cos(math.atan(math.sqrt(2)) / 2),
                - math.sin(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                + math.sin(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                0.0,
            ])
        )
        rw_a.st.m = 36e-3
        rw_a.st.I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([2.12e-6, 2.12e-6, 3e-3])
        )
        
        # rw B
        vehicle_model.cfg.obj_lst[2].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([10e-2, 10e-2, 5e-2])
        )
        vehicle_model.cfg.obj_lst[2].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([-0.2, -0.2, 0.2])
        )
        vehicle_model.cfg.obj_lst[2].q = numpy.ctypeslib.as_ctypes(
            numpy.array([
                math.cos(math.atan(math.sqrt(2)) / 2),
                + math.sin(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                - math.sin(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                0.0,
            ])
        )
        vehicle_model.st.obj_lst[2].m = 0.5
        vehicle_model.st.obj_lst[2].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([
                (3 * 10e-2 ** 2 + 5e-2 ** 2) / 12.0,
                (3 * 10e-2 ** 2 + 5e-2 ** 2) / 12.0,
                10e-2 ** 2 / 2.0
            ]) * 0.5
        )
        
        # rw C
        vehicle_model.cfg.obj_lst[3].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([10e-2, 10e-2, 5e-2])
        )
        vehicle_model.cfg.obj_lst[3].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.2, -0.2, -0.2])
        )
        vehicle_model.cfg.obj_lst[3].q = numpy.ctypeslib.as_ctypes(
            numpy.array([
                math.sin(math.atan(math.sqrt(2)) / 2),
                + math.cos(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                + math.cos(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                0.0,
            ])
        )
        vehicle_model.st.obj_lst[3].m = 0.5
        vehicle_model.st.obj_lst[3].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([
                (3 * 10e-2 ** 2 + 5e-2 ** 2) / 12.0,
                (3 * 10e-2 ** 2 + 5e-2 ** 2) / 12.0,
                10e-2 ** 2 / 2.0
            ]) * 0.5
        )
        
        # rw D
        vehicle_model.cfg.obj_lst[4].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([10e-2, 10e-2, 5e-2])
        )
        vehicle_model.cfg.obj_lst[4].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([-0.2, 0.2, -0.2])
        )
        vehicle_model.cfg.obj_lst[4].q = numpy.ctypeslib.as_ctypes(
            numpy.array([
                math.sin(math.atan(math.sqrt(2)) / 2),
                - math.cos(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                - math.cos(math.atan(math.sqrt(2)) / 2) / math.sqrt(2),
                0.0,
            ])
        )
        vehicle_model.st.obj_lst[4].m = 0.5
        vehicle_model.st.obj_lst[4].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([
                (3 * 10e-2 ** 2 + 5e-2 ** 2) / 12.0,
                (3 * 10e-2 ** 2 + 5e-2 ** 2) / 12.0,
                10e-2 ** 2 / 2.0
            ]) * 0.5
        )
        
        # mt A
        vehicle_model.cfg.obj_lst[5].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([2e-2, 2e-2, 20e-2])
        )
        vehicle_model.cfg.obj_lst[5].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.2, 0.2, 0.0])
        )
        vehicle_model.cfg.obj_lst[5].q = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 0.0, 0.0, 0.0])
        )
        vehicle_model.st.obj_lst[5].m = 0.1
        vehicle_model.st.obj_lst[5].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([
                (3 * 2e-2 ** 2 + 20e-2 ** 2) / 12.0,
                (3 * 2e-2 ** 2 + 20e-2 ** 2) / 12.0,
                2e-2 ** 2 / 2.0
            ]) * 0.1
        )
        
        # mt B
        vehicle_model.cfg.obj_lst[6].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([2e-2, 2e-2, 20e-2])
        )
        vehicle_model.cfg.obj_lst[6].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.0, -0.2, 0.2])
        )
        vehicle_model.cfg.obj_lst[6].q = numpy.ctypeslib.as_ctypes(
            numpy.array([math.sqrt(2) / 2, 0.0, - math.sqrt(2) / 2, 0.0])
        )
        vehicle_model.st.obj_lst[6].m = 0.1
        vehicle_model.st.obj_lst[6].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([
                (3 * 2e-2 ** 2 + 20e-2 ** 2) / 12.0,
                (3 * 2e-2 ** 2 + 20e-2 ** 2) / 12.0,
                2e-2 ** 2 / 2.0
            ]) * 0.1
        )
        
        # mt C
        vehicle_model.cfg.obj_lst[7].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([2e-2, 2e-2, 20e-2])
        )
        vehicle_model.cfg.obj_lst[7].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([-0.2, 0.0, -0.2])
        )
        vehicle_model.cfg.obj_lst[7].q = numpy.ctypeslib.as_ctypes(
            numpy.array([math.sqrt(2) / 2, - math.sqrt(2) / 2, 0.0, 0.0])
        )
        vehicle_model.st.obj_lst[7].m = 0.1
        vehicle_model.st.obj_lst[7].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([
                (3 * 2e-2 ** 2 + 20e-2 ** 2) / 12.0,
                (3 * 2e-2 ** 2 + 20e-2 ** 2) / 12.0,
                2e-2 ** 2 / 2.0
            ]) * 0.1
        )
        
        # sa A
        vehicle_model.cfg.obj_lst[8].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 1e-2, 0.5])
        )
        vehicle_model.cfg.obj_lst[8].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([+1.0, 0.0, 0.0])
        )
        vehicle_model.cfg.obj_lst[8].q = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 0.0, 0.0, 0.0])
        )
        vehicle_model.st.obj_lst[8].m = 1.0
        vehicle_model.st.obj_lst[8].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 1e-4, 0.25]) / 12
        )
        
        # sa B
        vehicle_model.cfg.obj_lst[9].bbox = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 1e-2, 0.5])
        )
        vehicle_model.cfg.obj_lst[9].r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([-1.0, 0.0, 0.0])
        )
        vehicle_model.cfg.obj_lst[9].q = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 0.0, 0.0, 0.0])
        )
        vehicle_model.st.obj_lst[9].m = 1.0
        vehicle_model.st.obj_lst[9].I_cm = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 1e-4, 0.25]) / 12
        )
        
        vehicle_model.st.clk.t = 0.0
        vehicle_model.st.sys.r_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([7.0e6, 0.0, 0.0])
        )
        vehicle_model.st.sys.q = numpy.ctypeslib.as_ctypes(
            numpy.array([1.0, 0.0, 0.0, 0.0])
        )
        vehicle_model.st.sys.v_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.0, 8.0e3, 0.0])
        )
        vehicle_model.st.sys.om_bar = numpy.ctypeslib.as_ctypes(
            numpy.array([0.0, 0.0, 0.0]) * math.pi / 360.0 / math.sqrt(3.0)
        )
        vehicle_model.ch.clk.t = 6.0
    return vehicle_model


def run(frame, line, quivers, shapes, rdata, qdata, pdata, vehicle_model, console):
    with console:
        r_bar = numpy.ctypeslib.as_array(vehicle_model.st.sys.r_bar)
        q = numpy.ctypeslib.as_array(vehicle_model.st.sys.q)
        # v_bar = numpy.ctypeslib.as_array(vehicle_model.st.sys.v_bar)
        # om_bar = numpy.ctypeslib.as_array(vehicle_model.st.sys.om_bar)
        if 1495 < vehicle_model.st.clk.t < 1505:
            print("change", vehicle_model.st.clk.t, ch_t.obj_t._T.E_ST)
            vehicle_model.ch.obj_lst[0].T = ch_t.obj_t._T.E_ST
            vehicle_model.ch.obj_lst[0].st.p_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 80e3])
            )
            vehicle_model.ch.obj_lst[1].T = ch_t.obj_t._T.E_ST
            vehicle_model.ch.obj_lst[1].st.h_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 1e-2])
            )
        if 2995 < vehicle_model.st.clk.t < 3005:
            print("change", vehicle_model.st.clk.t, ch_t.obj_t._T.E_NA)
            vehicle_model.ch.obj_lst[1].T = ch_t.obj_t._T.E_ST
            vehicle_model.ch.obj_lst[1].st.h_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, -1e-2])
            )
        if 4495 < vehicle_model.st.clk.t < 4505:
            print("change", vehicle_model.st.clk.t, ch_t.obj_t._T.E_EM)
            vehicle_model.ch.obj_lst[5].T = ch_t.obj_t._T.E_EM
            vehicle_model.ch.obj_lst[5].em.m_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 0.1])
            )
        if 5995 < vehicle_model.st.clk.t < 6005:
            print("change", vehicle_model.st.clk.t, ch_t.obj_t._T.E_NA)
            vehicle_model.ch.obj_lst[5].T = ch_t.obj_t._T.E_EM
            vehicle_model.ch.obj_lst[5].em.m_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 0])
            )
        if 7495 < vehicle_model.st.clk.t < 7505:
            print("change", vehicle_model.st.clk.t, ch_t.obj_t._T.E_IN)
            vehicle_model.ch.obj_lst[0].T = ch_t.obj_t._T.E_IN
            vehicle_model.ch.obj_lst[0].in_.F_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 40.0])
            )
        if 8995 < vehicle_model.st.clk.t < 9005:
            print("change", vehicle_model.st.clk.t, ch_t.obj_t._T.E_NA)
            vehicle_model.ch.obj_lst[0].T = ch_t.obj_t._T.E_IN
            vehicle_model.ch.obj_lst[0].in_.F_bar = numpy.ctypeslib.as_ctypes(
                numpy.array([0.0, 0.0, 0.0])
            )
        vehicle_model.ch.clk.t += 6.0
    # for each frame, update the data stored on each artist.
    rdata[frame % 1500] = r_bar / 1e6
    rdata[(frame + 1) % 1500] = numpy.nan
    R = scipy.spatial.transform.Rotation.from_quat(
        numpy.array([q[1], q[2], q[3], q[0]])
    )
    qdata[0] = R.apply(i_hat)
    qdata[1] = R.apply(j_hat)
    qdata[2] = R.apply(k_hat)
    # update the scatter plot:
    # update the line plot:
    line.set_data_3d(rdata[:, 0], rdata[:, 1], rdata[:, 2])
    quivers[0].set_segments([
        [
            [
                rdata[frame % 1500, 0],
                rdata[frame % 1500, 1],
                rdata[frame % 1500, 2]
            ],
            [
                rdata[frame % 1500, 0] + qdata[i, 0],
                rdata[frame % 1500, 1] + qdata[i, 1],
                rdata[frame % 1500, 2] + qdata[i, 2]
            ]
        ]
        for i in range(3)
    ])
    quivers[1].set_segments([
        [numpy.zeros((3,)), qdata[i]]
        for i in range(3)
    ])
    shapes.set_verts([
        R.apply(pdata[i])
        for i in range(len(pdata))
    ])
    return (line, *quivers, shapes)


def main():
    console = EpicycleConsole("/my.shm")
    try:
        console.open()
        vehicle_model = setup(console)
        
        rdata = numpy.empty((1500, 3))
        rdata[:] = numpy.nan
        qdata = numpy.eye(3)
        
        fig = pyplot.figure()
        ax1 = fig.add_subplot(1, 2, 1, projection="3d")
        ax2 = fig.add_subplot(1, 2, 2, projection="3d")
        line = ax1.plot(rdata[:, 0], rdata[:, 1], rdata[:, 2], color="0.5")[0]
        quivers = (
            ax1.quiver(
                rdata[0, numpy.newaxis, 0], rdata[0, numpy.newaxis, 1], rdata[0, numpy.newaxis, 2],
                qdata[0], qdata[1], qdata[2],
                color=["r", "g", "b"],
            ),
            ax2.quiver(
                numpy.zeros((3,)), numpy.zeros((3,)), numpy.zeros((3,)),
                qdata[0], qdata[1], qdata[2],
                color=["r", "g", "b"],
            ),
        )
        pdata = []
        for idx in range(vehicle_model.size):
            pdata.extend([
                numpy.array([
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                ]),
                numpy.array([
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                ]),
                numpy.array([
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                ]),
                numpy.array([
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                ]),
                numpy.array([
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                ]),
                numpy.array([
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [+ vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                    [- vehicle_model.cfg.obj_lst[idx].bbox[0] / 2, + vehicle_model.cfg.obj_lst[idx].bbox[1] / 2, - vehicle_model.cfg.obj_lst[idx].bbox[2] / 2],
                ])
            ])
            q = numpy.ctypeslib.as_array(vehicle_model.cfg.obj_lst[idx].q)
            R = scipy.spatial.transform.Rotation.from_quat(
                numpy.array([q[1], q[2], q[3], q[0]])
            )
            for jdx in range(6):
                pdata[6*idx+jdx] = R.apply(pdata[6*idx+jdx]) + vehicle_model.cfg.obj_lst[idx].r_bar
        shapes = mpl_toolkits.mplot3d.art3d.Poly3DCollection(pdata, alpha=0.5, ec="k")
        ax2.add_collection3d(shapes)
        ax1.set_xlim3d(-10, 10)
        ax1.set_ylim3d(-10, 10)
        ax1.set_zlim3d(-10, 10)
        ax2.set_xlim3d(-1, 1)
        ax2.set_ylim3d(-1, 1)
        ax2.set_zlim3d(-1, 1)
        ani = matplotlib.animation.FuncAnimation(
            fig=fig, func=run, fargs=(line, quivers, shapes, rdata, qdata, pdata, vehicle_model, console),
            interval=50,
            cache_frame_data=False,
        )
        pyplot.show()
    finally:
        console.close()


if __name__ == "__main__":
    main()

