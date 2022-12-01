load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_test(
    name = "day01",
    srcs = ["day01.cc"],
    deps = [
        "@gtest//:gtest_main",
    ],
    data = [":input_files"],
)

filegroup(
    name = "input_files",
    srcs = [
        "example_input.txt",
        "puzzle_input.txt",
    ],
)