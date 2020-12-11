package(default_visibility = ["//visibility:public"],)

cc_binary(
    name = "hls_cleanup",
    srcs = [
        "hls_cleanup.cc",
    ],
    deps = [
        "@boost//:algorithm",
        "@boost//:lexical_cast",
        "@boost//:filesystem",
        "@com_google_absl//absl/strings:strings",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
    ],
)

cc_binary(
    name = "video_merge",
    srcs = [
        "video_merge.cc",
    ],
    deps = [
        "@boost//:filesystem",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
    ],
    copts = ["-Wno-unused-result"],
)
