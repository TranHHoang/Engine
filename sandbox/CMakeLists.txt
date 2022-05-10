add_executable(sandbox "sandbox/Main.cc")

target_link_libraries(sandbox PRIVATE libcore)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:sandbox>/assets"
)

if(EMSCRIPTEN)
  # set_target_properties(sandbox PROPERTIES COMPILE_FLAGS "-pthread")
  set_target_properties(sandbox PROPERTIES LINK_FLAGS 
    "-sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2")
endif()