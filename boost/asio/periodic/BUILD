load("//tools:cpplint.bzl", "cpplint")

cc_library(
    name = "periodic",
    hdrs = glob(["*.h"]),
    srcs = glob(["*.cc"], exclude = ["*_test.cc"],),
    deps = [
        "@thirdparty//:boost",
        "@com_github_google_glog//:glog",
    ],
    linkopts = ["-pthread"],
    visibility = [
        "//visibility:public",
    ],
)

#cc_test(
#   name = "periodic_test",
    #srcs = glob(["*_test.cc"]),
#   deps = [
#       "@com_google_googletest//:gtest_main",
#       ":periodic",
#   ],
#   visibility = [
#       "//visibility:public",
#   ],
#)

cpplint()
