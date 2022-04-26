add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ".")
target_sources(stb INTERFACE "stb/stb_image.c")