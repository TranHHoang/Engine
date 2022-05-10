#pragma once

#ifdef EMSCRIPTEN
#include <GLES3/gl32.h>
#elif GLES
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif