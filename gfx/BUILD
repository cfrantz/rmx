package(default_visibility=["//visibility:public"])

cc_library(
    name = "shader",
    srcs = [ "shader.cc" ],
    hdrs = [ "shader.h" ],
    deps = [
        "//util:file",
        "//util:logging",
    ],
)

cc_library(
    name = "color",
    hdrs = [ "color.h" ],
    srcs = [ "color.cc" ],
    deps = [
        "@glm_git//glm",
    ],
)

cc_library(
    name = "canvas",
    srcs = [ "canvas.cc" ],
    hdrs = [ "canvas.h" ],
    deps = [
        ":shader",
        "@glm_git//glm",
    ],
)

cc_library(
    name = "camera",
    srcs = [ "camera.cc" ],
    hdrs = [ "camera.h" ],
    deps = [
        "@glm_git//:glm",
    ],
)

cc_library(
    name = "raymarch",
    srcs = [ "raymarch.cc" ],
    hdrs = [ "raymarch.h" ],
    deps = [
        ":camera",
        ":shader",
        "@glm_git//:glm",
    ],
)

cc_library(
    name = "swmarch",
    srcs = [ "swmarch.cc" ],
    hdrs = [ "swmarch.h" ],
    deps = [
        "//imwidget:glbitmap",
        "@glm_git//:glm",
    ],
)

