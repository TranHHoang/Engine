#pragma once

#ifdef EMSCRIPTEN
#include <GLES3/gl32.h>
#elif ANDROID
#include <GLES3/gl31.h>
#else
#include <glad/gl.h>
#endif