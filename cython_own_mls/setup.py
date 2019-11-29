from Cython.Build import cythonize,build_ext

from distutils.core import setup
from distutils.extension import Extension

extensions = [Extension(
    'pymls',
    ["pymls.pyx","mls.cpp"],
    include_dirs = ["include/"],
    libraries = ["pymls"],
    language = 'c++',
)]


setup(
    name = "mls_emm",
    cmdclass = {"build_ext":build_ext},
    ext_modules = extensions,
)
