#!/usr/bin/env python  
  
from distutils.core import setup, Extension  

MOD = 'testModule'  
setup(name=MOD, ext_modules=[
    Extension(
        MOD, sources=['extension_test/pycall_extention.c']
        )]
    ) 