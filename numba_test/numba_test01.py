from numba import jit
import numpy as np
import time
 
'''
    example 01
'''
x = np.arange(100).reshape(10, 10)
def add_string(a): 
    trace = 0
    for i in range(a.shape[0]):   # 
        trace += np.tanh(a[i, i]) 
    return a + trace 

@jit(nopython=True) # Set "nopython" mode for best performance
def add_string_numba(a): # Function is compiled to machine code when called the first time
    trace = 0
    for i in range(a.shape[0]):   # Numba likes loops
        trace += np.tanh(a[i, i]) # Numba likes NumPy functions
    return a + trace              # Numba likes NumPy broadcasting

st = time.time()
print(add_string(x))
print('no numba',(time.time() - st)*1000)
st00 = time.time()
print(add_string(x))
print('no numba',(time.time() - st00)*1000)

st1 = time.time()
print(add_string_numba(x))          #编译
print('with numba',(time.time() - st1)*1000)
st11 = time.time()
print(add_string_numba(x))
print('with numba',(time.time() - st11)*1000)

# import pandas as pd

# x = {'a': [1, 2, 3], 'b': [20, 30, 40]}
 
# @jit
# def use_pandas(a): # Function will not benefit from Numba jit
#     df = pd.DataFrame.from_dict(a) # Numba doesn't know about pd.DataFrame
#     df += 1                        # Numba doesn't understand what this is
#     return df.cov()                # or this!
 
'''
    example 02
'''

import time
from numba import jit
 
def num():
 
    arr = []
    for i in range(10000000):
        arr.append(i)
 
stime = time.time()
num()
etime = time.time() - stime
# print(arr)
print('num用时:{}秒'.format(etime))

@jit
def num_numba():
 
    arr = []
    for i in range(10000000):
        arr.append(i)
 
stime = time.time()
num_numba()
etime = time.time() - stime
# print(arr)
print('num_numba用时:{}秒'.format(etime))