package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cpp2util",
    hdrs = ["include/cpp2util.h"],
    includes = ["include/"],
)

cc_binary(
    name = "cppfront_compiler",
    srcs = glob(["source/*.cpp"]) + glob(["source/*.h"]),
    copts = ["-std=c++20"],
)