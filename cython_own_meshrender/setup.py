from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
# import numpy

extension = Extension(
        'mesh_render',
        ["mesh_render.pyx", "mesh_render/mesh_render.cpp"],
        include_dirs = ["mesh_render/"],
        libraries = ["mesh_render/mesh_render"],
        language = 'c++',
)

setup(
    name = "mesh_render_emm",
    ext_modules = cythonize([extension]),
)