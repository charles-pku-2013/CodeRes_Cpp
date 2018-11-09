#! /bin/bash

# 独立编译op lib，不需要tf源码。供调试和单元测试用
# 需要系统中安装有预编译好的tensorflow，通过 pip install tensorflow 安装即可

set -x

# rm -f *.so

TF_CFLAGS=( $(python -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_compile_flags()))') )
TF_LFLAGS=( $(python -c 'import tensorflow as tf; print(" ".join(tf.sysconfig.get_link_flags()))') )
g++ -std=c++11 -shared tokenizer.cc level_dnn_utils.cc -o tokenizer.so -fPIC ${TF_CFLAGS[@]} ${TF_LFLAGS[@]} -Wall -O3
