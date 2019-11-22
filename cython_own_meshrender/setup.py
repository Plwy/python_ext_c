from distutils.core import setup
from disutils.extension import Extension
from Cython.Build import cythonize

extension = Extension(
        'mesh_render',
        ["mesh_render.pyx", "mesh_render/mesh_render.c"],
        include_dirs = ["mesh_render/mesh_render"]
        libraries = ["mesh_render/mesh_render"]
)

setup(
    name = "mesh_render_emm",
    ext_modules = cythonize(extension)
)