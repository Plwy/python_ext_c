def addtest(a,b):
    cdef float c=a+b
    return c

def tran(double x):
    cdef double ret
    ret = x**2 - x
    return ret