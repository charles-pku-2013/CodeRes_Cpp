package(default_visibility = ["//visibility:public"],)

cc_binary(
    name = "qvr_watcher",
    srcs = [
        "qvr_watcher.cc",
    ],
    deps = [
        "//tools:run_cmd",
        "@boost//:algorithm",
        "@boost//:filesystem",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "qvr_watcher",
    headers = ["boost/include"],
    srcs = [
        "liboost_thread.a",
    ],
    deps = [
        "//tools:run_cmd",
        "@boost//:algorithm",
        "@boost//:filesystem",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
    ],
)
cc_binary(
    name = "qvr_streamer",
    srcs = [
        "qvr_streamer.cc",
    ],
    deps = [
        "//tools:run_cmd",
        "@boost//:algorithm",
        "@boost//:filesystem",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
    ],
)
