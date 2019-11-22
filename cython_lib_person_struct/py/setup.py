# from distutils.core import setup, Extension
# from Cython.Build import cythonize

# extension = Extension(
#     "person",
#     ["person.pyx"],
#     libraries=["person"]
# )

# setup(
#     ext_modules=cythonize([extension])
# )
from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "person",
    ["person.pyx", "../cperson/person.c"],
    libraries=["../cperson/person"],
    include_dirs=["../cperson/"]
)

setup(
    ext_modules=cythonize([extension])
)