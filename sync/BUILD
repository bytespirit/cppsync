cc_library(
    name = "lock",
    hdrs = ["lock.h"],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "event",
    hdrs = ["event.h"],
    deps = [
        ":lock",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "barrier",
    hdrs = ["barrier.h"],
    deps = [
        ":lock",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "semaphore",
    hdrs = ["semaphore.h"],
    deps = [
        ":lock",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "signal",
    hdrs = ["signal.h"],
    deps = [
        ":lock",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "waiting_queue",
    hdrs = ["waiting_queue.h"],
    srcs = ["waiting_queue.cc"],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "sync",
    hdrs = ["sync.h"],
    deps = [
        ":lock",
        ":barrier",
        ":semaphore",
        ":signal",
        ":waiting_queue",
    ],
    copts = ["-std=c++1z"],
    visibility = ["//visibility:public"],
)
