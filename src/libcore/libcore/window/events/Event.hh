#pragma once
#include <variant>

#include <libcore/window/events/AppEvent.hh>
#include <libcore/window/events/KeyEvent.hh>
#include <libcore/window/events/MouseEvent.hh>

namespace Engine::Window::Event {
using EventType = std::variant<WindowInitialized,
                               WindowResized,
                               WindowClosed,
                               WindowMoved,
                               KeyDown,
                               KeyUp,
                               TextInput,
                               MouseMoved,
                               MouseScrolled,
                               MouseButtonDown,
                               MouseButtonUp>;
} // namespace Engine::Window::Event