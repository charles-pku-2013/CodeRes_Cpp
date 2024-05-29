package(default_visibility = ["//visibility:public"],)

cc_binary(
    name = "test",
    srcs = [
        "test.cc",
    ],
    deps = [
        # "@boost//:lexical_cast",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_github_google_glog//:glog",
        "@jsoncpp_git//:jsoncpp",
        "@jd_hocon//lib:jd-cpp-hocon",
    ],
)

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
        "@com_google_absl//absl/time:time",
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

cc_binary(
    name = "str_elem_cmp",
    srcs = [
        "str_elem_cmp.cc",
    ],
    deps = [
        # "//tools:run_cmd",
        # "@boost//:algorithm",
        # "@boost//:filesystem",
        "@com_github_gflags_gflags//:gflags",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
    ],
)
