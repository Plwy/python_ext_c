cimport cqueue
from cpython.ref cimport PyObject

# 类叫做Queue，不怕和C的Queue结构体名字冲突了
cdef class Queue:
    cdef cqueue.Queue *_c_queue

    def __cinit__(self):
        self._c_queue = cqueue.queue_new()

    def __dealloc__(self):
        if self._c_queue is not NULL:
            cqueue.queue_free(self._c_queue)


    def push_tail(self, v):
        if (cqueue.queue_push_tail(self._c_queue, <void*>v) != 0):
            (<PyObject*>v).ob_refcnt += 1
        else:
            raise MemoryError()

    def pop_tail(self):
        cdef cqueue.QueueValue v
        v = cqueue.queue_pop_tail(self._c_queue)
        if v != NULL:
            obj = <object>v
            (<PyObject*>obj).ob_refcnt -= 1
            return obj
        else:
            return None