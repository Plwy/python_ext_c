# from distutils.core import setup, Extension
# from Cython.Build import cythonize

# extension = Extension(
#     "queue",
#     ["queue.pyx"],
#     libraries=["cqueue"]
# )

# setup(
#     ext_modules=cythonize([extension])
# )

from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "queue",
    ["queue.pyx","cqueue/queue.c"],
    libraries=["cqueue/cqueue"],
    include_dirs=["cqueue/"]
)

setup(
    ext_modules=cythonize([extension])
)
