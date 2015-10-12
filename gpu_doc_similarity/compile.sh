#!/bin/sh

#nvcc -o test work.cpp work_impl.cpp gpu.cu -lpthread -lboost_system -O3 -DFLOAT64 -D_DEBUG -gencode arch=compute_30,code=sm_30
nvcc -gencode arch=compute_30,code=sm_30 -o test work.cpp work_impl.cpp gpu.cu -lpthread -lboost_system -O3 -DFLOAT64 -D_DEBUG



