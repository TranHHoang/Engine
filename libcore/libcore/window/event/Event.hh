#pragma once
#include <variant>

#include <libcore/window/event/AppEvent.hh>
#include <libcore/window/event/KeyEvent.hh>
#include <libcore/window/event/MouseEvent.hh>

namespace Engine::Window::Event {
using EventType = std::variant<WindowResized,
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