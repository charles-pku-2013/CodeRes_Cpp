# cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME rest_server)
project(${PROJECT_NAME})
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果brpc未安装，可以指定其源码路径
# set(BRPC_DIR "/Devel/jd/brpc-1.8.0/CMakeLists.txt" CACHE PATH "brpc directory")
# find_package(brpc REQUIRED)

file(GLOB_RECURSE SRCS_LISTS
    ${PROJECT_SOURCE_DIR}/*.cc
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBRPC_WITH_GLOG=0 -DGFLAGS_NS=google -DBTHREAD_USE_FAST_PTHREAD_MUTEX -D_GNU_SOURCE -DUSE_SYMBOLIZE -DNO_TCMALLOC -D__STDC_FORMAT_MACROS -DSTDC_LIMIT_MACROS -D__const__=__unused__ -DNDEBUG -O2 -pthread -pipe -Wall -W -fPIC -fstrict-aliasing -Wno-invalid-offsetof -Wno-unused-parameter -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME} ${SRCS_LISTS})
target_link_libraries(${PROJECT_NAME} brpc gflags protobuf dl rt)

