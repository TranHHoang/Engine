#pragma once
#include <cstdint>
#include <optional>

#include <libcore/renderer/Shader.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::OpenGL {
class Shader : public Engine::Renderer::Shader {
public:
  Shader(const UniformLayout& layout);
  ~Shader();

  uint32_t programID() const { return _programID; }

  void setMat4(std::string_view name, const Mat4& mat) override;
  int maxTextureSlots() const override;

private:
  std::string preprocessShader(GLuint type) const;
  std::optional<GLuint> buildShaderProgram() const;
  std::optional<GLuint> compileShader(GLuint shaderType) const;

private:
  uint32_t _programID;
};
} // namespace Engine::Renderer::OpenGL