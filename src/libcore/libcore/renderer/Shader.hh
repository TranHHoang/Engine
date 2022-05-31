#pragma once
#include <string_view>

#include <libcore/lib/Math.hh>
#include <libcore/renderer/UniformLayout.hh>

namespace Engine::Renderer::Shader {
class Shader {
public:
public:
  Shader(const UniformLayout& layout) : _layout{layout} {}
  virtual ~Shader() = default;

  virtual void setupLayout() {}
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void setMat4(std::string_view name, const Mat4& mat) = 0;
  virtual int maxTextureSlots() const = 0;

private:
  UniformLayout _layout;
};
} // namespace Engine::Renderer::Shader