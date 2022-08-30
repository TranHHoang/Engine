#pragma once
#include <string_view>

#include <libcore/lib/Math.hh>
#include <libcore/renderer/UniformLayout.hh>

namespace Engine::Renderer {
class Shader {
public:
  Shader(const UniformLayout& layout) : _layout{layout} {}
  virtual ~Shader() = default;

  virtual void setMat4(std::string_view name, const Mat4& mat) = 0;
  virtual int maxTextureSlots() const = 0;

private:
  UniformLayout _layout;
};
} // namespace Engine::Renderer