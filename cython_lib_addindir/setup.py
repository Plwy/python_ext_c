from distutils.core import setup, Extension
from Cython.Build import cythonize

# extension = Extension(
#     "pymath",
#     ["pymath.pyx"],
#     libraries=["cmath_lib/cmath"]
# )

# setup(
#     ext_modules=cythonize([extension])
# )

from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "pymath",
    ["pymath.pyx", "cmath_lib/cmath.c"],
    include_dirs = ["cmath_lib/"],
    # library_dirs=["cmath_lib/"],
    libraries = ["cmath_lib/cmath"]
)

setup(
    ext_modules=cythonize([extension])
)