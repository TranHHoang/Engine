#pragma once
#include <string>
#include <string_view>

namespace Engine::Window {
struct Props {
  std::string title;
  unsigned width;
  unsigned height;

  Props(std::string_view title = "Engine",
        unsigned width = 1024,
        unsigned height = 768)
      : title{title},
        width{width},
        height{height} {}
};
} // namespace Engine::Window