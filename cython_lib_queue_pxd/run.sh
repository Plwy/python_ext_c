#!/bin/sh

set -xe

cd cqueue
gcc -fPIC -shared -g *.c -o libcqueue.so

cd ..
CFLAGS="-I`pwd`/cqueue/" LDFLAGS="-L`pwd`/cqueue/" python3 setup.py build_ext --inplace

LD_LIBRARY_PATH=`pwd`/cqueue/ python3 test.py
