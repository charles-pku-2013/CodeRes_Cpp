package(default_visibility = ["//visibility:public"],)

cc_library(
    name = "kernel",
    hdrs = [
        "kernel.h",
    ],
    deps = [
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_library(
    name = "kernel_factory",
    hdrs = [
        "kernel_factory.h",
    ],
    deps = [
        ":kernel",
        "@com_google_absl//absl/strings:strings",
    ],
)

cc_library(
    name = "demo_plugin",
    hdrs = [
        "demo_plugin.h",
    ],
    srcs = [
        "demo_plugin.cc",
    ],
    deps = [
        ":kernel_factory",
        "@com_github_google_glog//:glog",
    ],
)

# How to build .so shared lib
# If you specify both linkopts=['-static'] and linkshared=True, you get a single completely self-contained unit. If you specify both linkstatic=1 and linkshared=True, you get a single, mostly self-contained unit.
# cc_binary里不支持hdrs。标准写法将所用到的.h .cc 写成 cc_libary 然后在cc_binary里添加依赖
cc_binary(
    name = "libplugin.so",  # NOTE 这里不可以命名为 libdemo_plugin.so 现有的编译项 cc_library demo_plugin 也会生成同样名字的文件，有冲突
    deps = [
        ":demo_plugin",
    ],
    copts = [
        "-fPIC",
    ],
    linkshared = True,
    linkstatic = 1,
)

cc_binary(
    name = "demo",
    srcs = [
        "main.cc",
    ],
    deps = [
        ":kernel_factory",
        "@com_github_google_glog//:glog",
    ],
    linkopts = [
        "-ldl",
    ],
)

