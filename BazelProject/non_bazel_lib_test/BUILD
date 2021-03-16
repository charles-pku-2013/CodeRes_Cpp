cc_binary(
    name = "openblas_test",
    srcs = [
        "openblas_test.cc",
    ],
    deps = [
        "@openblas//:openblas",
    ],
    copts = ["-Wno-unused-result"],
    # linkopts = ["-lpthread"],
)

cc_binary(
    name = "libevent_test",
    srcs = [
        "libevent_test.cc",
    ],
    deps = [
        "@com_github_libevent_libevent//:libevent",
    ],
    copts = ["-Wno-unused-result", "-fpermissive"],
)
