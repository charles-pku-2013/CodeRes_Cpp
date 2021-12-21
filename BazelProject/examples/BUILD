package(default_visibility = ["//visibility:public"],)

cc_binary(
    name = "macro_demo",
    srcs = [
        "macro_demo.cc",
    ],
)

cc_binary(
    name = "absl_test",
    srcs = [
        "absl_test.cc",
    ],
    deps = [
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
    ],
)

cc_binary(
    name = "thread_pool_test",
    srcs = [
        "thread_pool_test.cc",
    ],
    deps = [
        "//tools:thread_pool",
    ],
)

cc_binary(
    name = "gflags_demo",
    srcs = [
        "gflags_demo.cc",
    ],
    deps = [
        "@com_github_gflags_gflags//:gflags",
    ],
)
