#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
int fac(int n)  
{  
    if (n < 2) return(1);  
    return (n)*fac(n-1);  
}  
  
char *reverse(char *s)  
{  
    register char t,  
            *p = s,  
            *q = (s + (strlen(s) - 1));  
  
    while (s && (p < q))  
    {  
        t = *p;  
        *p++ = *q;  
        *q-- = t;  
    }  
    return(s);  
}  
  
int test()  
{  
    char s[BUFSIZ];  
    printf("4! == %d\n", fac(4));  
    printf("8! == %d\n", fac(8));  
    printf("12! == %d\n", fac(12));  
    strcpy(s, "abcdef");  
    printf("reversing 'abcdef', we get '%s'\n", \  
        reverse(s));  
    strcpy(s, "madam");  
    printf("reversing 'madam', we get '%s'\n", \  
        reverse(s));  
    return 0;  
}  

int add(int a, int b)
{
    if(a > b)
    {
        a = a + 1;
        return a;
    }else{
        b = b + a;
        return b;
    }
}

/*
a、包含Python的头文件；
b、为每个模块的每一个函数增加一个型如PyObject* Module_func()的包装函数；
c、为每个模块增加一个型如PyMethodDef ModuleMethods[]的数组；
d、增加模块初始化函数void initModule()。
*/
#include "Python.h"  
  
static PyObject *  
testModule_fac(PyObject *self, PyObject *args)  
{  
    int num;  
    if (!PyArg_ParseTuple(args, "i", &num))  
        return NULL;  
    return (PyObject*)Py_BuildValue("i", fac(num));  
}  
  
static PyObject *  
testModule_reverse(PyObject *self, PyObject *args)  
{  
    char *orig_str;  
    char *dupe_str;  
    PyObject* retval;  
  
    if (!PyArg_ParseTuple(args, "s", &orig_str))  
        return NULL;  
    retval = (PyObject*)Py_BuildValue("ss", orig_str,  
        dupe_str=reverse(strdup(orig_str)));  
    free(dupe_str);             
    return retval;  
}  
  
static PyObject *  
testModule_test(PyObject *self, PyObject *args)  
{  
    test();  
    return (PyObject*)Py_BuildValue("");  
}  

static PyObject *  
testModule_add(PyObject *self, PyObject *args)  
{  
    int num1;
    int num2;
    if (!PyArg_ParseTuple(args, "ii", &num1, &num2))  
        return NULL;  
    return (PyObject*)Py_BuildValue("i", add(num1, num2));  
}  
  

static PyMethodDef  
testModule_Methods[] =  
{  
    { "fac", testModule_fac, METH_VARARGS },  
    { "reverse", testModule_reverse, METH_VARARGS },  
    { "test", testModule_test, METH_VARARGS },  
    { "add", testModule_add, METH_VARARGS },  
    { NULL, NULL },  
};  
  
// void initExtest()  
// {  
//     Py_InitModule("Extest", ExtestMethods);  
// }

static struct PyModuleDef testModule = {
    PyModuleDef_HEAD_INIT,
    "testModule",   /* name of module */
    "", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    testModule_Methods
};

PyMODINIT_FUNC PyInit_testModule(void)
{
    return PyModule_Create(&testModule);
}

// PyMODINIT_FUNC initExtest(void)
// {
    
//     // Py_InitModule("Extest", ExtestMethods, "a module");
//     PyImport_AppendInittab("Extest", PyInit_spam);
//     Py_Initialize();
//     PyImport_ImportModule("Extest");
// }

