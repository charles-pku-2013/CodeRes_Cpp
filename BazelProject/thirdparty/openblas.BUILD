package(
    default_visibility = ["//visibility:public"],
)

# licenses(["notice"])  # BSD

# exports_files(["LICENSE"])

lib_files = [
    "openblas/lib64/libopenblas.a",
]
include_files = glob(["openblas/include/*.h"])

genrule(
    name = "openblas-srcs",
    # srcs = ["//:non_bazel_lib_test/compile.sh"],
    outs = include_files + lib_files,
    cmd = "\n".join([
# https://docs.bazel.build/versions/master/be/make-variables.html#predefined_variables
# pwd: /root/.cache/bazel/_bazel_root/6b498043c3554c8234ef8edd683063a7/execroot/BazelProject
# SRC: /root/.cache/bazel/_bazel_root/6b498043c3554c8234ef8edd683063a7/execroot/BazelProject/external/openblas
# $(@D): bazel-out/k8-opt/genfiles/external/openblas/openblas/lib64
        "export SRC_DIR=$$(pwd)/external/openblas",
        "export INSTALL_DIR=$$(pwd)/$(@D)/..",
        # "echo $$(pwd) > /tmp/test.log",
        # "echo $$INSTALL_DIR >> /tmp/test.log",
        # "echo $$SRC_DIR >> /tmp/test.log",
        # "echo $(@D) >> /tmp/test.log",
        "cd $$SRC_DIR",
        "mkdir build && cd build",
        "cmake3 -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_SHARED_LIBS=OFF CMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$$INSTALL_DIR ..",
        "make -j",
        "make install",
        # "make PREFIX=$$INSTALL_DIR install",
    ]),
)

cc_library(
    name = "openblas",
    hdrs = include_files,
    # srcs = [
        # "openblas/lib/libopenblas.a",
    # ],
    srcs = lib_files,
    includes = ["openblas/include"],
    # deps = [
        # ":openblas-srcs",
    # ],
    linkopts = ["-lpthread"],
    linkstatic = 1,
)

# filegroup(
    # name = "openblas-files",
    # srcs = include_files + lib_files,
# )
