from Cython.Build import cythonize,build_ext

from distutils.core import setup
from distutils.extension import Extension
import numpy
import subprocess

proc_opencv_libs = subprocess.check_output("pkg-config --libs opencv".split())
proc_opencv_incs = subprocess.check_output("pkg-config --cflags opencv".split())
# print(proc_opencv_libs)

opencv_libs = [lib for lib in str(proc_opencv_libs).split()]
opencv_libs = opencv_libs[1:]
opencv_libs[-1] = opencv_libs[-1][:-3]

extensions = [
    Extension(
        'pymls',  
        ["pymls.pyx", "mls.cpp"],  
        language='c++',  #
        include_dirs=["include/",
        "/usr/local/include/",
         numpy.get_include()],  
        extra_link_args= opencv_libs
    )
]


setup(
    name = "mls_emm",
    ext_modules = cythonize(extensions)
)
