from distutils.core import setup
from Cython.Build import cythonize

setup(
    name = "ohmyeyes",
    ext_modules=cythonize("hello_world.pyx")
)
