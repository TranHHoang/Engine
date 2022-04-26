add_library(
    glad
    "include/glad/gl.h"
    "include/KHR/khrplatform.h"
    "src/gl.c"
)

if(LINUX)
  target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
endif()

target_include_directories(glad PUBLIC "include")