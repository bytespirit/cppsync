cc_test(
    name = "event-test",
    srcs = ["event-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//cppsync:event",
    ],
)

cc_test(
    name = "barrier-test",
    srcs = ["barrier-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//cppsync:barrier",
    ],
)

cc_test(
    name = "semaphore-test",
    srcs = ["semaphore-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//cppsync:semaphore",
    ],
)

cc_test(
    name = "signal-test",
    srcs = ["signal-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//cppsync:signal",
    ],
)
