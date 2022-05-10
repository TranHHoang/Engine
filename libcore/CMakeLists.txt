file(GLOB LIBCORE_SOURCES CONFIGURE_DEPENDS
  "libcore/*.cc"
  "libcore/lib/*.cc"
  "libcore/ecs/*.cc"
  "libcore/renderer/*.cc"
  "libcore/window/*.cc"
)
file(GLOB LIBCORE_HEADERS CONFIGURE_DEPENDS 
  "libcore/*.hh"
  "libcore/lib/*.hh"
  "libcore/ecs/*.hh"
  "libcore/renderer/*.hh"
  "libcore/window/*.hh"
  "libcore/window/event/*.hh"
  "libcore/window/input/*.hh"
)

set(LIBCORE_SOURCES_PLATFORM "")
set(LIBCORE_HEADERS_PLATFORM "")

if(WINDOWS)
  file(GLOB LIBCORE_SOURCES_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/windows/*.cc")
  file(GLOB LIBCORE_HEADERS_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/windows/*.hh")
elseif(LINUX)
  file(GLOB LIBCORE_SOURCES_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/linux/*.cc")
  file(GLOB LIBCORE_HEADERS_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/linux/*.hh")
elseif(MACOS)
  file(GLOB LIBCORE_SOURCES_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/macos/*.mm")
  file(GLOB LIBCORE_HEADERS_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/macos/*.hh")
elseif(EMSCRIPTEN)
  file(GLOB LIBCORE_SOURCES_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/emscripten/*.cc")
  file(GLOB LIBCORE_HEADERS_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/platform/emscripten/*.hh")
endif()

set(LIBCORE_RENDERER_SOURCES "")
set(LIBCORE_RENDERER_HEADERS "")

file(GLOB_RECURSE LIBCORE_RENDERER_SOURCES CONFIGURE_DEPENDS "libcore/renderer/opengl/*.cc")
file(GLOB_RECURSE LIBCORE_RENDERER_HEADERS CONFIGURE_DEPENDS "libcore/renderer/opengl/*.hh")

list(APPEND LIBCORE_SOURCES ${LIBCORE_SOURCES_PLATFORM} ${LIBCORE_RENDERER_SOURCES})
list(APPEND LIBCORE_HEADERS ${LIBCORE_HEADERS_PLATFORM} ${LIBCORE_RENDERER_HEADERS})

message(STATUS ${LIBCORE_SOURCES})

add_library(libcore STATIC ${LIBCORE_SOURCES} ${LIBCORE_HEADERS})

target_compile_features(libcore PUBLIC cxx_std_20)
# target_precompile_headers(libcore PUBLIC "PCH.hh")

set(LINK_LIBS glad glm stb fmt::fmt EnTT::EnTT)
set(INCLUDE_DIRS ".")

if(NOT EMSCRIPTEN)
  list(APPEND LINK_LIBS OpenGL::GL)

  if(LINUX)
    list(APPEND LINK_LIBS Threads::Threads ${X11_LIBRARIES})
  elseif(MACOS)
    list(APPEND LINK_LIBS ${COCOA_LIBRARY})
  endif()
else()
  list(APPEND INCLUDE_DIRS "${EMSCRIPTEN_SYSROOT}/include")
endif()

target_include_directories(libcore PUBLIC ${INCLUDE_DIRS})
target_link_libraries(libcore PUBLIC ${LINK_LIBS})

if(EMSCRIPTEN)
  # set_target_properties(libcore PROPERTIES COMPILE_FLAGS "-pthread")
  set_target_properties(libcore PROPERTIES LINK_FLAGS 
    "-sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2")
endif()