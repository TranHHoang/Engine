#pragma once

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
#elif IOS
#include <libcore/window/platforms/ios/Factory.hh>
using WindowFactory = Engine::Window::IOSFactory;
#else
#include <emscripten/emscripten.h>
#include <libcore/window/platforms/emscripten/Factory.hh>
using WindowFactory = Engine::Window::EmscriptenFactory;
#endif