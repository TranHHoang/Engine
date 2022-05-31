#pragma once

#define SINGLE_THREAD false
#define WAIT_FOR_WINDOW_INIT false

#ifdef WINDOWS
#include <libcore/window/platforms/windows/Factory.hh>
using WindowFactory = Engine::Window::WindowsFactory;
#elif LINUX
#include <libcore/window/platforms/linux/Factory.hh>
using WindowFactory = Engine::Window::LinuxFactory;
#elif MACOS
#include <libcore/window/platforms/macos/Factory.hh>
using WindowFactory = Engine::Window::MacOSFactory;
#elif ANDROID
#include <libcore/window/platforms/android/Factory.hh>
using WindowFactory = Engine::Window::AndroidFactory;
#undef WAIT_FOR_WINDOW_INIT
#define WAIT_FOR_WINDOW_INIT true
#else
#include <emscripten/emscripten.h>
#include <libcore/window/platforms/emscripten/Factory.hh>
using WindowFactory = Engine::Window::EmscriptenFactory;
#define SINGLE_THREAD true
#endif