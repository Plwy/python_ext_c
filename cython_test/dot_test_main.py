import numpy as np
import time

import dot_cython

def naive_dot(a, b):
    if a.shape[1] != b.shape[0]:
        raise ValueError('shape not matched')
    n, p, m = a.shape[0], a.shape[1], b.shape[1]
    c = np.zeros((n, m), dtype=np.float32)
    for i in range(n):
        for j in range(m):
            s = 0
            for k in range(p):
                s += a[i, k] * b[k, j]
            c[i, j] = s
    return c

if __name__ == '__main__':
    a = np.random.randint(0,10,(100,200))
    b = np.random.randint(0,10,(200,50))
    print(a)
    print(b)

    st = time.time()
    c = naive_dot(a,b)    
    end1 = time.time()
    print((end1 - st)*1000*1000)

    a = np.array(a, dtype = np.float32)
    b = np.array(b, dtype = np.float32)
    c = dot_cython.naive_dot(a, b)
    end2 = time.time()
    print((end2 - end1)*1000*1000)
