cmake_minimum_required(VERSION 3.16)

project(Engine VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EMSCRIPTEN)
  set(GLES TRUE)
  add_compile_definitions(EMSCRIPTEN GLES)
elseif(WIN32)
  set(WINDOWS TRUE)
  add_compile_definitions(WINDOWS)
elseif(ANDROID)
  set(GLES TRUE)
  add_compile_definitions(ANDROID GLES)
elseif(UNIX AND NOT APPLE)
  set(LINUX TRUE)
  add_compile_definitions(LINUX)
elseif(IOS)
  add_compile_definitions(IOS GLES)
elseif(UNIX AND APPLE)
  set(MACOS TRUE)
  add_compile_definitions(MACOS)
endif()

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

if(NOT EMSCRIPTEN AND NOT ANDROID AND NOT IOS)
  # Packages
  message("==== Checking OpenGL ====")
  find_package(OpenGL REQUIRED)

  if(LINUX)
    message("==== Checking X11 ====")
    find_package(X11 REQUIRED)
    message("==== Checking Threads ====")
    find_package(Threads REQUIRED)
  elseif(MACOS)
    message("==== Checking Cocoa ====")
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
  endif()
endif()

if(IOS)
  message("==== Checking UIKit ====")
  find_library(UIKIT_LIBRARY UIKit REQUIRED)
  message("==== Checking GLKit ====")
  find_library(GLKIT_LIBRARY GLKit REQUIRED)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/projects/ios/Lib)
endif()

# Bundle libs
message("==== Checking GLAD ====")
add_subdirectory("src/vendor/glad")

message("==== Checking stb ====")
add_subdirectory("src/vendor/stb")

if(ANDROID)
  message("==== Checking android_native_app_glue ====")
  add_subdirectory("src/vendor/android_native_app_glue")
endif()

# Third party libs
include(FetchContent)

# ==== GLM ====
message("==== Checking GLM ====")
FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG "0.9.9.8"
)
FetchContent_MakeAvailable(glm)

# ==== fmt ====
message("==== Checking fmt ====")
FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
  GIT_TAG "8.1.1"
)
FetchContent_MakeAvailable(fmt)

# ==== EnTT ====
message("==== Checking EnTT ====")
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY "https://github.com/skypjack/entt"
  GIT_TAG "v3.10.1"
)
FetchContent_MakeAvailable(EnTT)

add_subdirectory("src/libcore")
add_subdirectory("src/sandbox")