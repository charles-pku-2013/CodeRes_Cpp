# pip install pybind11
# python setup.py build

# 等价于
# https://github.com/charlotteLive/pybind11-Chinese-docs/blob/main/src/04.%E9%A6%96%E6%AC%A1%E5%B0%9D%E8%AF%95.md
# c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)

from setuptools import setup, Extension

functions_module = Extension(
    name='example',
    sources=['example.cpp'],
    extra_compile_args=["-O3","-fPIC"],

    # How to get include path
    # import site
    # print(site.getsitepackages())
    # Or
    # `python3 -m pybind11 --includes`
    include_dirs=['/usr/local/lib/python3.8/dist-packages/pybind11/include'],
)

setup(ext_modules=[functions_module])


#  Usage
#  cd build/lib.linux-x86_64-3.8
#  python3
#  import example
#  example.inadd()

