cmake_minimum_required(VERSION 3.16)

project(Engine VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(WINDOWS TRUE)
    add_compile_definitions(WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    add_compile_definitions(LINUX)
elseif(UNIX AND APPLE)
    set(MACOS TRUE)
    add_compile_definitions(MACOS)
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Packages
message(STATUS "==== Checking OpenGL ====")
find_package(OpenGL REQUIRED)

if(LINUX)
message(STATUS "==== Checking X11 ====")
find_package(X11 REQUIRED)

message(STATUS "==== Checking Threads ====")
find_package(Threads REQUIRED)
elseif(MACOS)
message(STATUS "==== Checking Cocoa ====")
find_library(COCOA_LIBRARY Cocoa)
endif()

# Bundle libs
message(STATUS "==== Checking GLAD ====")
add_subdirectory("vendor/glad")

message(STATUS "==== Checking stb ====")
add_subdirectory("vendor/stb")

# Third party libs
include(FetchContent)

# ==== GLM ====
message(STATUS "==== Checking GLM ====")
FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "0.9.9.8"
)
FetchContent_MakeAvailable(glm)

# ==== fmt ====
message(STATUS "==== Checking fmt ====")
FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
    GIT_TAG "8.1.1"
)
FetchContent_MakeAvailable(fmt)

# ==== EnTT ====
message(STATUS "==== Checking EnTT ====")
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY "https://github.com/skypjack/entt"
    GIT_TAG "v3.10.0"
)
FetchContent_MakeAvailable(EnTT)

add_subdirectory("libcore")
add_subdirectory("sandbox")