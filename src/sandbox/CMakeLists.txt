set(SANDBOX_SOURCES "sandbox/Main.cc")

if(ANDROID)
  add_library(sandbox SHARED ${SANDBOX_SOURCES})
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
elseif(IOS)
  add_library(sandbox STATIC ${SANDBOX_SOURCES})
else()
  add_executable(sandbox ${SANDBOX_SOURCES})
endif()

target_link_libraries(sandbox PRIVATE libcore)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/assets" "$<TARGET_FILE_DIR:sandbox>/assets"
)

if(EMSCRIPTEN)
  set_target_properties(sandbox PROPERTIES COMPILE_FLAGS "-pthread")
  set_target_properties(sandbox PROPERTIES LINK_FLAGS 
    "-sUSE_WEBGL2 
    -sMIN_WEBGL_VERSION=2 
    -sMAX_WEBGL_VERSION=2 
    -sOFFSCREENCANVAS_SUPPORT 
    -sOFFSCREEN_FRAMEBUFFER 
    -sGL_DEBUG=1 
    -sUSE_PTHREADS 
    -sPTHREAD_POOL_SIZE=1")
endif()