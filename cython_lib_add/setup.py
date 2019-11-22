# from distutils.core import setup, Extension
# from Cython.Distutils import build_ext
# import numpy

# sourcefile = ["pymath.pyx", "cmath.c"]

# setup(
# 	name = 'pymath',
#     cmdclass={'build_ext': build_ext},
#     ext_modules=[Extension("pymath",
#                  sourcefile,
#                  language='c')],
# )

from distutils.core import setup, Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

sourcefile = ["pymath.pyx", "cmath.c"]
extension = Extension(
    "pymath",
    sourcefile,
    language='c'
    
)

setup(
	name = 'pymath',
    cmdclass={'build_ext': build_ext},
    ext_modules=cythonize([extension])
)
