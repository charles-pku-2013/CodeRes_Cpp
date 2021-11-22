package(default_visibility = ["//visibility:public"],)

cc_library(
    name = "run_cmd",
    hdrs = [
        "run_cmd.h",
    ],
    srcs = [
        "run_cmd.cc",
    ],
    deps = [
        "@boost//:iostreams",
    ],
)

cc_library(
    name = "thread_pool",
    hdrs = [
        "thread_pool.h",
    ],
    deps = [
        "@com_google_absl//absl/synchronization:synchronization",
        "@com_google_absl//absl/base:core_headers",
    ],
)
