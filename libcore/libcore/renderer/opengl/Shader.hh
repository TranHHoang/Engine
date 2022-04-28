#pragma once
#include <cstdint>
#include <optional>

#include <glad/gl.h>

#include <libcore/renderer/Shader.hh>

namespace Engine::Renderer::Shader {
class OpenGLShader : public Shader {
public:
  OpenGLShader(const UniformLayout& layout);
  ~OpenGLShader();

  void bind() const override;
  void unbind() const override;
  void setMat4(std::string_view name, const Mat4& mat) override;

  uint32_t programID() const { return _programID; }

private:
  std::optional<GLuint> buildShaderProgram(std::string_view vertexSrc,
                                           std::string_view fragmentSrc) const;
  std::optional<GLuint> compileShader(GLuint shaderType,
                                      std::string_view src) const;

private:
  uint32_t _programID;
};
} // namespace Engine::Renderer::Shader