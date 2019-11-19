import ctypes  

#调用gcc编译生成的动态库

ll = ctypes.cdll.LoadLibrary   
lib = ll("ctype_test/libpycall.so")    
lib.foo(1, 3)  
print ('***finish***'  )