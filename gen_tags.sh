#!/bin/bash
ctags -f .tags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extras=+q --excmd=number .
ctags -f .tfs.tags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extras=+q --excmd=number ../tensorflow_serving_lib
ctags -f .jcommon.tags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extras=+q --excmd=number ../jcommon
ctags -f .brpc.tags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extras=+q --excmd=number ../brpc
ctags -f .absl.tags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extras=+q --excmd=number ../abseil-cpp/absl
