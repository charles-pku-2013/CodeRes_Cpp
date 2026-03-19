cmake_minimum_required(VERSION 3.15)
project(FollyEventDemo CXX)

# Folly 强依赖 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 告诉 CMake 优先寻找和使用静态库 (.a)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# 【核心】强制静态链接 C++ 标准库和 GCC 运行时，防止目标机器缺少 libstdc++.so.6
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

# 寻找 Folly (vcpkg 会自动接管这一步)
find_package(folly CONFIG REQUIRED)

# 编译可执行文件
add_executable(app folly.cpp)

# 链接 Folly 及其所有依赖
target_link_libraries(app PRIVATE Folly::folly)
