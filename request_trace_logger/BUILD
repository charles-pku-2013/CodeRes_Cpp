package(default_visibility = ["//visibility:public"])

cc_library(
    name = "request_trace_logger",
    hdrs = ["request_trace_logger.h"],
    srcs = ["request_trace_logger.cc"],
    deps = [
        "//common/canvas:canvas",
        "@boost//:log",
        "@boost//:lexical_cast",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
        "@jsoncpp_git//:jsoncpp",
        "@jd_hocon//lib:jd-cpp-hocon",
    ],
)

# build in some other project like jds_merger
# bazelisk build -c opt @jcommon//common/request_trace_logger:request_trace_logger_test
cc_binary(
    name = "request_trace_logger_test",
    srcs = ["request_trace_logger_test.cc"],
    deps = [
        ":request_trace_logger",
    ],
)
