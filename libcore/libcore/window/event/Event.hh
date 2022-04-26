#pragma once
#include <variant>

#include <libcore/window/event/AppEvent.hh>
#include <libcore/window/event/KeyEvent.hh>
#include <libcore/window/event/MouseEvent.hh>

namespace Engine {
template <typename... Ts> // (7)
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;
} // namespace Engine

namespace Engine::Window::Event {

using EventType = std::variant<std::monostate,
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