
## setup.py
该项目的几种setup写法。
1.
from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "pymath",
    ["pymath.pyx", "cmath_lib/cmath.c"]
)

setup(
    ext_modules=cythonize([extension])
)


执行：
    CFLAGS="-I`pwd`/cmath_lib/" LDFLAGS="-L`pwd`/cmath_lib/" python3 setup.py build_ext --inplace
    python3 cmath_test_main.py 

不指定CFLAG，LDFLAG会报错：
    #include "cmath.h"
            ^~~~~~~~~
    compilation terminated.
    error: command 'x86_64-linux-gnu-gcc' failed with exit status 1

指定头文件（.h文件）的路径，如：CFLAGS=-I/usr/include -I/path/include。（如果你的头文件不处于gcc默认的路径之中，就需要使用此参数指定。）
指定非默认路径的库文件的位置。用法：LDFLAGS=”-L/usr/lib -L/path/to/your/lib”。

2.
from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "pymath",
    ["pymath.pyx"],        #此处改动
    libraries=["cmath"]    #此处改动
)

setup(
    ext_modules=cythonize([extension])
)

不指定CFLAG，LDFLAG会报错。

执行：
CFLAGS="-I`pwd`/cmath_lib/" LDFLAGS="-L`pwd`/cmath_lib/" python3 setup.py build_ext --inplace
LD_LIBRARY_PATH=`pwd`/cmath_lib/ python3 cmath_test_main.py


****
需要指定LD_LIBRARY_PATH=`pwd`/cmath_lib/ 否则报错
Traceback (most recent call last):
  File "cmath_test_main.py", line 1, in <module>
    import pymath
ImportError: libcmath.so: cannot open shared object file: No such file or directory

即找不到指定的cmath库, 需要执行时给出LD_LIBRARY_PATH路径。
****

3.
如果不想执行python3 cmath_test_main.py时指定LD_LIBRARY_PATH，可在setup中指定libraries时给定详细路径，如下：
from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "pymath",
    ["pymath.pyx"],
    libraries=["cmath_lib/cmath"] #此处改动
)
setup(
    ext_modules=cythonize([extension])
)


执行：
CFLAGS="-I`pwd`/cmath_lib/" LDFLAGS="-L`pwd`/cmath_lib/" python3 setup.py build_ext --inplace
python3 cmath_test_main.py



怎样才能不需要指定CFLAG，LDFLAG，直接setup呢？？
希望在setup.py中设置好头文件和库，而不需要执行时指定。

4.
可以在setup.py中设置include_dirs和library_dirs，如下：
from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "pymath",
    ["pymath.pyx", "cmath_lib/cmath.c"],
    include_dirs = ["cmath_lib/"],
    library_dirs=["cmath_lib/"]
)

setup(
    ext_modules=cythonize([extension])
)


简单执行：
python3 setup.py build_ext --inplace
python3 cmath_test_main.py


也可以直接不指定库文件目录只接指定库文件library_dirs=["cmath_lib/"] 替换为libraries = ["cmath_lib/cmath"]。
执行方法一样。

综上所述，问题的出现主要在于清楚的指明可执行项目的头文件查找路径和库文件查找路径



