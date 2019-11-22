## use
cd cython_hello
python3 setup.py build_ext -i
python3 hello_test.py

## issue
### detail
单独编译.pyx，生成hello_world.c，
然后编译hello_world.c生成.so库,
测试时import出现错误。
'''
    cython -3 hello_world.pyx

    gcc -shared -fPIC hello_world.c -o hahahello.so -I/usr/include/python3.6/

报错：
import hahahello
ImportError: dynamic module does not define module export function (PyInit_hahahello)
???

### solve：
emmm...so包的名字得和.pyx .c一致，否则会出错。
