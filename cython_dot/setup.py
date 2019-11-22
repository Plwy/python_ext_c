from distutils.core import setup, Extension
from Cython.Build import cythonize
import numpy
setup(ext_modules = cythonize(Extension(
    'dot_cython',  #要生成的动态链接库的名字
     sources=['dot_cython.pyx'],  #可以包含 .pyx 文件和 .c / .cpp 文件
     language='c',  #
     include_dirs=[numpy.get_include()],  #传给 gcc 的 -I 参数
     library_dirs=[],  #传给 gcc 的 -L 参数
    libraries=[],
    extra_compile_args=[],
    extra_link_args=[]
)))