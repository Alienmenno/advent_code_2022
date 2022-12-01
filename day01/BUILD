load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("//thirdparty/cppfront:cppfront_rules.bzl", "cpp2_binary")

cc_test(
    name = "day01",
    srcs = ["day01.cc"],
    data = [
        "example_input.txt",
        "puzzle_input.txt",
    ],
    deps = ["@gtest//:gtest_main"],
)