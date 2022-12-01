def _cpp2_to_cpp(srcs):
    return [src[:-5]+".cpp" for src in srcs]

def _cpp2_binary_impl(ctx):
    ctx.actions.run(
        outputs = [ctx.outputs.out],
        inputs = ctx.files.srcs,
        executable = "(@cppfront//:cppfront_compiler)",
    )
    #  declare_file
    return DefaultInfo(executable = ctx.outputs.out)
    # native.genrule(
    #     name = ctx.label.name + "_cpp2_to_cc",
    #     srcs = ctx.files.srcs,
    #     outs = cc_files,
    #     cmd = ("$(location @cppfront//:cppfront) " + " ".join(ctx.attr.flags)),
    #     tools = ["@cppfront//:cppfront"],
    #     visibility = ["//visibility:private"],
    # )

    # native.cc_binary(
    #     name = ctx.label.name,
    #     srcs = cc_files,
    #     deps = ["@cppfront//:cpp2util"] + ctx.attr.deps,
    # )


_cpp2_binary = rule(
    implementation = _cpp2_binary_impl,
    executable = True,
    attrs = {
        "srcs": attr.label_list(allow_files = [".cpp2"]),
        "out": attr.output(),
        "deps": attr.label_list(),
        "data": attr.label_list(allow_files = True),
        "flags": attr.string_list(),
    },
)

def cpp2_binary(**kwargs):
    outputs = _cpp2_to_cpp(kwargs["srcs"])
    _cpp2_binary(
        out = "pure_hello.cpp",
        **kwargs
    )

# def cpp2_binary(name, flags = [], srcs = [], **kwargs):
#     """Like cc_library, but for .cpp2 files

#     This is a simple macro that takes .cpp2 files, runs them through cppfront,
#     then sticks the resulting files in a cc_library.

#     flags: Flags to stick in the cppfront command line
#     srcs: .cpp2 files to compile into C++ files
#     All other arguments are passed into the cc_library rule.
#     """

#     cc_files = _cpp2_to_cpp(srcs)

#     # The cppfront binary only supports outputting output files to the cwd. As
#     # such, we have to use this hacky solution to copy them from the exec root
#     # (typically in a temporary sandbox) into the location where they are
#     # supposed to go.
#     # Hopefully cppfront will add a -o flag or something so that we can remove
#     # the copy.
#     native.genrule(
#         name = name + "_cpp2_to_cc",
#         srcs = srcs,
#         outs = cc_files,
#         cmd = ("$(location @cppfront//:cppfront) " + " ".join(flags) + " $<;" +
#                "cp " + " ".join(cc_files) + "  $(@D)"),
#         tools = ["@cppfront//:cppfront"],
#         visibility = ["//visibility:private"],
#     )

#     native.cc_binary(
#         name = name,
#         srcs = cc_files,
#         deps = ["@cppfront//:cpp2util"] + kwargs.get("deps", default = []),
#         **kwargs
#     )
