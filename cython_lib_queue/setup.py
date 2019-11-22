from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "queue",
    ["queue.pyx", "cqueue/queue.c"],
    # libraries=["cqueue/cqueue"],
    library_dirs=["cqueue/"],
    include_dirs=["cqueue/"]
)

setup(
    ext_modules=cythonize([extension])
)

# 编译可直接执行
# python3 setup.py build_ext --inplace
# python3 test.py