cc_test(
    name = "event-test",
    srcs = ["event-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//cppsync:event",
    ],
)
