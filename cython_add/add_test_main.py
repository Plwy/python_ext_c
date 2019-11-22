'''
cython -3 addtest.pyx
gcc -c -fPIC -I/usr/include/python3.6 addtest.c
gcc -shared addtest.o -o addtest.so
'''

import addtest

print(addtest.addtest(10,22))
print(addtest.tran(33))