#!/bin/sh

set -xe
cd c
gcc -fPIC -shared -g *.c -o libfoo.so

cd ../py
CFLAGS="-I`pwd`/../c/" LDFLAGS="-L`pwd`/../c/" python3 setup.py build_ext --inplace

LD_LIBRARY_PATH=`pwd`/../c/ python3 test.py
