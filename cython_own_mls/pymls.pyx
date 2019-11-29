# distutils: language=c++
# distutils: sources = mls.cpp
cimport pymls
cimport cython
from cpython.ref cimport PyObject
from libc.stdlib cimport malloc, free
cimport numpy as np


def mls_deformation(int height, int width,
                    oriImg, dstImg,
                    x1, y1, int n,
                    x2, y2):

    cdef int n_x1  = len(x1)
    cdef int *c_x1 = <int*>malloc(n_x1 * sizeof(int))
    for i in range(n_x1):
        c_x1[i] = x1[i]

    cdef int n_y1 = len(y1)
    cdef int *c_y1 = <int*>malloc(n_y1 * sizeof(int))
    for i in range(n_y1):
        c_y1[i] = y1[i]

    cdef int n_x2 = len(x2)
    cdef int *c_x2 = <int*>malloc(n_x2 * sizeof(int))
    for i in range(n_x2):
        c_x2[i] = x2[i]

    cdef int n_y2 = len(y2)
    cdef int *c_y2 = <int*>malloc(n_y2 * sizeof(int))
    for i in range(n_y2):
        c_y2[i] = y2[i]

    cdef int n_ori = len(oriImg)
    cdef unsigned char* c_ori_img = <unsigned char*>malloc(n_ori * sizeof(unsigned char))
    for i in range(n_ori):
        c_ori_img = oriImg[i]


    cdef int n_dst = len(dstImg)
    cdef unsigned char* c_dst_img = <unsigned char*>malloc(n_dst * sizeof(unsigned char))
    for i in range(n_dst):
        c_dst_img = dstImg[i]

    pymls.cmls_deformation(height, width,
                        c_ori_img, c_dst_img,
                        c_x1, c_y1, n,
                        c_x2, c_y2) 

    free(c_x1)
    free(c_y1)
    free(c_x2)
    free(c_y2)  
    free(c_ori_img)
    free(c_dst_img)  


