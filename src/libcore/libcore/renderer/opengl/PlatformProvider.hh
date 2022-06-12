#pragma once
#include <any>
#include <functional>

#include <libcore/renderer/PlatformProvider.hh>

namespace Engine::Renderer::OpenGL {
struct PlatformProvider : public Engine::Renderer::PlatformProvider {
  std::function<std::any()> createContext;
  std::function<void(std::any)> destroyContext;
};
} // namespace Engine::Renderer::OpenGL