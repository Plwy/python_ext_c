import numba as nb
import numpy as np
import time

# 普通的 for
def add1(x, c):
    rs = [0.] * len(x)
    for i, xx in enumerate(x):
        rs[i] = xx + c
    return rs

# list comprehension
def add2(x, c):
    return [xx + c for xx in x]

# 使用 jit 加速后的 for
@nb.jit(nopython=True)
def add_with_jit(x, c):
    rs = [0.] * len(x)
    for i, xx in enumerate(x):
        rs[i] = xx + c
    return rs

y = np.random.random(10**5).astype(np.float32)
x = y.tolist()

# assert np.allclose(add1(x, 1), add2(x, 1), add_with_jit(x, 1))

st00 = time.time()
add1(x, 1)
st01 = time.time()
print("add1", (st01-st00)*1000)

add2(x, 1)
st02 = time.time()
print("add1", (st02-st01)*1000)

add_with_jit(x, 1)
st03 = time.time()
print("add1", (st03-st02)*1000)

add_with_jit(x, 1)
st04 = time.time()
print("add1", (st04-st03)*1000)
# print(np.allclose(wrong_add(x, 1), 1))