import ctypes  

#Python调用C++(类)动态链接库
so = ctypes.cdll.LoadLibrary   
lib = so("ctype_test_cpp/libpycallclass.so")  

print ('display()') 
lib.display()  
print ('display(100)')  
lib.display_int(100)