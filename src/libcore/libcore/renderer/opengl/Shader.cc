#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/File.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/Shader.hh>
#include <libcore/renderer/opengl/ShaderSource.hh>

namespace Engine::Renderer::OpenGL {
Shader::Shader(const UniformLayout& layout) : Renderer::Shader{layout} {
  if (auto programID = buildShaderProgram()) {
    _programID = programID.value();
    Logger::info("Number of texture units available: {}", maxTextureSlots());
  } else {
    Logger::error("Failed to create OpenGL shader program");
    assert(false);
  }
}

Shader::~Shader() {
  glDeleteProgram(_programID);
}

void Shader::bind() const {
  glUseProgram(_programID);
}

void Shader::unbind() const {
  glUseProgram(0);
}

void Shader::setMat4(std::string_view name, const Mat4& mat) {
  GLint location = glGetUniformLocation(_programID, name.data());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

int Shader::maxTextureSlots() const {
  GLint textureUnits;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
  return static_cast<int>(textureUnits);
}

std::string Shader::preprocessShader(GLuint type) const {
  using namespace fmt::literals;
  if (type == GL_VERTEX_SHADER) {
    std::string_view version =
#ifdef GLES
        "#version 300 es";
#else
        "#version 330 core";
#endif
    return fmt::format(vertexSrc, "version"_a = version);
  } else {
    int maxSlots = maxTextureSlots();
    std::string_view version =
#ifdef GLES
        "#version 300 es\nprecision mediump float;";
#else
        "#version 330 core";
#endif
    std::string outputColor = "switch (id) {\n";
    for (int i = 0; i < maxSlots; i++) {
      outputColor += fmt::format(
          "case {0}:\n"
          "o_Color = texture(u_Textures[{0}], v_TexCoord) * v_Color;\n"
          "break;\n",
          i);
    }
    outputColor += "}";
    return fmt::format(fragmentSrc,
                       "version"_a = version,
                       "maxTextureSlots"_a = maxTextureSlots(),
                       "outputColor"_a = outputColor);
  }
}

std::optional<GLuint> Shader::buildShaderProgram() const {
  auto vertexShader = compileShader(GL_VERTEX_SHADER);
  auto fragmentShader = compileShader(GL_FRAGMENT_SHADER);

  if (vertexShader && fragmentShader) {
    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShader.value());
    glAttachShader(programID, fragmentShader.value());
    glLinkProgram(programID);

    GLint isLinked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(programID, maxLength, &maxLength, infoLog.data());

      glDeleteProgram(programID);
      glDeleteShader(vertexShader.value());
      glDeleteShader(fragmentShader.value());

      Logger::error("{}", infoLog.data());
      assert(false && "OpenGL shader linking failure!");

      return {};
    }

    glDetachShader(programID, vertexShader.value());
    glDetachShader(programID, fragmentShader.value());

    return programID;
  }

  return {};
}

std::optional<GLuint> Shader::compileShader(GLuint shaderType) const {
  GLuint shader = glCreateShader(shaderType);

  std::string src = preprocessShader(shaderType);
  const GLchar* source = src.data();
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

    Logger::error("{}", infoLog.data());
    assert(false && "OpenGL shader compilation failure!");
    glDeleteShader(shader);

    return {};
  }

  return shader;
}
} // namespace Engine::Renderer::OpenGL