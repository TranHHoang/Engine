set(GLAD_SOURCES 
  "include/glad/gl.h"
  "src/gl.c")

if(GLES AND NOT EMSCRIPTEN)
  set(GLAD_SOURCES 
    "include/glad/gles2.h"
    "src/gles2.c")
endif()

add_library(glad ${GLAD_SOURCES})

if(LINUX)
  target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
endif()

target_include_directories(glad PUBLIC "include")