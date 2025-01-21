import os.path

from setuptools import setup, Extension
import numpy

_epicycle = Extension(
    "_epicycle",
    sources=[
        "epicycle.c",
    ],
    include_dirs=["./include"],
    define_macros=[
        ("__MEDIUM__", None),
        ("POLY_DEG", "5"),
    ],
    library_dirs=[os.path.join(os.path.dirname(__file__), "epicycle")],
    libraries=[
        "rt",
        "epibase",
        "epimath",
        "epicore",
        "epigee",
    ],
    runtime_library_dirs=[os.path.join(os.path.dirname(__file__), "epicycle")],
)

setup(
    ext_package="epicycle",
    ext_modules=[
        _epicycle,
    ]
)

