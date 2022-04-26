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
    "libcore/window/windows/*.cc")
  file(GLOB LIBCORE_HEADERS_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/windows/*.hh")
elseif(LINUX)
  file(GLOB LIBCORE_SOURCES_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/linux/*.cc")
  file(GLOB LIBCORE_HEADERS_PLATFORM CONFIGURE_DEPENDS 
    "libcore/window/linux/*.hh")
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

set(LINK_LIBS glad glm OpenGL::GL stb fmt::fmt EnTT::EnTT)

if(LINUX)
  list(APPEND LINK_LIBS Threads::Threads ${X11_LIBRARIES})
endif()

target_include_directories(libcore PUBLIC ".")
target_link_libraries(libcore PUBLIC ${LINK_LIBS})