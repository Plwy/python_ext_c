# python 调用 c

##　1.python 调用c / c++(类) 动态链接库
项目文件：
'ctype_test'
'ctype_test_cpp'

    gcc/g++ 编译生成动态库 xx.so　文件
    ’‘’
        gcc -o libpycall.so -shared -fPIC pycall.c
        g++ -o libpycallclass.so -shared -fPIC pycallclass.cpp
    ‘’‘



# 2.