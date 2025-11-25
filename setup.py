from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11
import sys

cpp_files = [
    "Core/bindings.cpp",
]

extra_compile_args = []
if sys.platform == 'darwin':
    extra_compile_args = ['-std=c++17', '-stdlib=libc++']
elif sys.platform == 'win32':
    extra_compile_args = ['/std:c++17']
else:
    extra_compile_args = ['-std=c++17']

ext_modules = [
    Pybind11Extension(
        "cache_sim",
        cpp_files,
        include_dirs=[
            pybind11.get_include(),
            "../Core",
        ],
        extra_compile_args=extra_compile_args,
    ),
]

setup(
    name="cache_sim",
    version="1.0.0",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)
