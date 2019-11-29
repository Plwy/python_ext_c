cdef extern from "mls.h":
    void cmls_deformation(int height, int width,
                         unsigned char *oriImg, 
                         unsigned char *dstImg, 
                         int *x1, int *y1, int n,
                         int *x2, int *y2)
