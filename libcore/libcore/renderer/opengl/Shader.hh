#pragma once
#include <cstdint>
#include <string_view>

#include <glad/gl.h>

#include <libcore/lib/Option.hh>
#include <libcore/renderer/Shader.hh>

namespace Engine::Renderer::Shader {
class OpenGLShader : public Shader {
public:
  OpenGLShader(const UniformLayout& layout);
  ~OpenGLShader();

  void bind() const override;
  void unbind() const override;
  virtual void setMat4(std::string_view name, const Mat4& mat) override;

  uint32_t programID() const { return _programID; }

private:
  Option<GLuint> buildShaderProgram(std::string_view vertexSrc,
                                    std::string_view fragmentSrc) const;
  Option<GLuint> compileShader(GLuint shader, std::string_view src) const;

private:
  uint32_t _programID;
};
} // namespace Engine::Renderer::Shader