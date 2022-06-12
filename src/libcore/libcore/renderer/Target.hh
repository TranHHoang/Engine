#pragma once
#include <cstdint>

#include <libcore/lib/Logger.hh>

namespace Engine::Renderer {
class Target {
public:
  struct Info {
    uint32_t width, height;
  };

public:
  Target(const Info& info) : _info{info} {}
  virtual ~Target() = default;

  void resize(uint32_t width, uint32_t height);

  virtual void bind() = 0;
  virtual void unbind() = 0;
  virtual void invalidate() = 0;

protected:
  Info _info;
};
} // namespace Engine::Renderer