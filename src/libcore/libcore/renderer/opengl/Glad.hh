#pragma once

#ifdef EMSCRIPTEN
#include <GLES3/gl32.h>
#elif ANDROID
#include <GLES3/gl31.h>
#elif IOS
#include <OpenGLES/ES3/gl.h>
#else
#include <glad/gl.h>
#endif