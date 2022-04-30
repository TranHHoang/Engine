#pragma once
#include <functional>

#include <libcore/renderer/PlatformProvider.hh>

namespace Engine::Renderer {
struct OpenGLPlatformProvider : public PlatformProvider {
  std::function<void*()> createContext;
  std::function<void(void*)> destroyContext;
};
} // namespace Engine::Renderer