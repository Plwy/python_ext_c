# from distutils.core import setup, Extension
# from Cython.Build import cythonize

# extension = Extension(
#     "foo",
#     ["foo.pyx"],
#     libraries=["foo"]
# )

# setup(
#     ext_modules=cythonize([extension])
# )

from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "foo",
    ["foo.pyx","../c/foo.c"],
    libraries=["../c/foo"],
    include_dirs=["../c/"]
)

setup(
    ext_modules=cythonize([extension])
)

