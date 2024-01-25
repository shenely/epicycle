from setuptools import setup, Extension

import numpy

_epicycle = Extension(
    "_epicycle",
    sources=[
        "epicycle.c",
    ],
    include_dirs=["./include"],
    library_dirs=["/home/shenely/Documents/epicycle/epicycle"],
    libraries=[
        "rt",
        "epibase",
        "epimath",
        "epicore",
        "epigee",
    ],
    runtime_library_dirs=["/home/shenely/Documents/epicycle/epicycle"],
)

setup(
    ext_package="epicycle",
    ext_modules=[
        _epicycle,
    ]
)

