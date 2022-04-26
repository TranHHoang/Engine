#pragma once
#include <string_view>

namespace Engine::Renderer {
enum class API { OpenGL, Vulkan, DirectX };
inline std::string_view toString(API api) {
  switch (api) {
  case API::OpenGL:
    return "OpenGL";
  case API::Vulkan:
    return "Vulkan";
  case API::DirectX:
    return "DirectX";
  }
  return "Unknown";
}

class Context {
public:
public:
  Context(API api) : _api{api} {}
  virtual ~Context() = default;

  virtual void destroy() = 0;

  API api() const { return _api; }

private:
  API _api;
};
} // namespace Engine::Renderer