# set the project name and version
cmake_minimum_required(VERSION 3.22)

project(TranslateServer)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find_path(BRPC_INCLUDE_PATH NAMES brpc/server.h)
# find_library(BRPC_LIB NAMES libbrpc.a brpc)
# if((NOT BRPC_INCLUDE_PATH) OR (NOT BRPC_LIB))
    # message(FATAL_ERROR "Fail to find brpc")
# endif()
# include_directories(${BRPC_INCLUDE_PATH})

# find_path(GFLAGS_INCLUDE_PATH gflags/gflags.h)
# find_library(GFLAGS_LIBRARY NAMES gflags libgflags)
# if((NOT GFLAGS_INCLUDE_PATH) OR (NOT GFLAGS_LIBRARY))
    # message(FATAL_ERROR "Fail to find gflags")
# endif()
# include_directories(${GFLAGS_INCLUDE_PATH})

# find_path(GLOG_INCLUDE_PATH NAMES glog/logging.h)
# find_library(GLOG_LIB NAMES glog)
# if((NOT GLOG_INCLUDE_PATH) OR (NOT GLOG_LIB))
    # message(FATAL_ERROR "Fail to find glog")
# endif()
# include_directories(${GLOG_INCLUDE_PATH})

# find_package(OpenSSL)
# include_directories(${OPENSSL_INCLUDE_DIR})

# include(FetchContent)
# FetchContent_Declare(
    # fmt
    # GIT_REPOSITORY https://github.com/fmtlib/fmt
    # GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
# FetchContent_MakeAvailable(fmt)

# find_package(fmt REQUIRED)
find_package(glog REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)

find_library(BRPC_LIB NAMES brpc)

set(BRPC_LIB ${BRPC_LIB} leveldb dl rt)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS translate.proto)
# include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})  # proto 生成的文件放在build目录下

file(GLOB SRC "*.cc")

# add the executable
add_executable(server ${SRC} ${PROTO_SRCS})

target_link_libraries(server
    ${BRPC_LIB}
    ${Protobuf_LIBRARIES}
    OpenSSL::SSL OpenSSL::Crypto
    fmt glog::glog curl
    sentencepiece ctranslate2
)

target_compile_options(server PUBLIC
    # -DNDEBUG
    -DBRPC_WITH_GLOG=1
    -DGFLAGS_NS=google
    -D__const__=__unused__
    -O2 -pipe -pthread -fPIC
    -fno-omit-frame-pointer
)
