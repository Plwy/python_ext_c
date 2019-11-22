#!/bin/sh

set -xe

cd cmath_lib
gcc -fPIC -shared -g *.c -o libcmath.so

cd ..
CFLAGS="-I`pwd`/cmath_lib/" LDFLAGS="-L`pwd`/cmath_lib/" python3 setup.py build_ext --inplace

LD_LIBRARY_PATH=`pwd`/cmath_lib/ python3 cmath_test_main.py