set(GLAD_SOURCES 
  "include/glad/gl.h"
  "src/gl.c")

add_library(glad ${GLAD_SOURCES})

if(LINUX)
  target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})
endif()

target_include_directories(glad PUBLIC "include")