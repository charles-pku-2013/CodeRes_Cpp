load("//:my_cc_combine.bzl", "my_cc_combine")

my_cc_combine(
    name = "lib_combine_so",
    # 这里将所有的静态库合并成一个静态库
    # genstatic = True,
    output = "libcombine.so",
    deps = [
        "//test/libA:libA",
        "//test/libB:libB",
        "//test/libC:libC",
    ],
)
