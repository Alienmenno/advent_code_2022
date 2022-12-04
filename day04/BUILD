load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_test(
    name = "day04",
    srcs = ["day04.cc"],
    data = [
        "example_input.txt",
        "puzzle_input.txt",
    ],
    deps = [
        "@gtest//:gtest_main",
        "@range-v3//:range-v3",
        "@fmt",
    ],
)