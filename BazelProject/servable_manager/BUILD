package(default_visibility = ["//visibility:public"],)

cc_library(
    name = "tool",
    hdrs = [
        "config_reader.h",
        "thread_pool.h",
        "timer_task.h",
    ],
    deps = [
        "@boost//:lexical_cast",
        "@boost//:asio",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/synchronization:synchronization",
    ],
)

cc_library(
    name = "servable_manager",
    hdrs = [
        "servable.h",
        "servable_manager.h",
    ],
    srcs = [
        "servable_manager.cc",
    ],
    deps = [
        ":tool",
        "@boost//:filesystem",
        "@boost//:thread",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_library(
    name = "demo_dict",
    hdrs = [
        "demo_dict.h",
    ],
    srcs = [
        "demo_dict.cc",
    ],
    deps = [
        ":servable_manager",
    ],
    alwayslink = 1,
)

cc_binary(
    name = "test",
    srcs = [
        "test.cc",
    ],
    deps = [
        ":demo_dict",
        "@com_github_google_glog//:glog",
        "@com_github_gflags_gflags//:gflags",
    ],
    linkopts = ["-pthread"],
)
