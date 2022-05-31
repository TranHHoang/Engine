#pragma once
#include <string>
#include <string_view>

#include <libcore/renderer/Context.hh>

namespace Engine::Window {
struct Props {
  Renderer::API api;
  std::string title;
  unsigned width;
  unsigned height;

  Props(Renderer::API api = Renderer::API::OpenGL,
        std::string_view title = "Engine",
        unsigned width = 1024,
        unsigned height = 768)
      : api{api},
        title{title},
        width{width},
        height{height} {}
};
} // namespace Engine::Window