#include <string>
#include <vector>

#include <fmt/core.h>
#include <glm/gtc/type_ptr.hpp>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/File.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/renderer/opengl/Shader.hh>

namespace Engine::Renderer::Shader {
OpenGLShader::OpenGLShader(const UniformLayout& layout)
    : Shader::Shader{layout} {
  std::string vertexSrc = readFile("assets/shaders/opengl/Shader.vert");
  std::string fragmentSrc = readFile("assets/shaders/opengl/Shader.frag");

  if (auto programID = buildShaderProgram(vertexSrc, fragmentSrc)) {
    _programID = programID.value();
  } else {
    Logger::error("Failed to create OpenGL shader program");
    assert(false);
  }
}

OpenGLShader::~OpenGLShader() {
  glDeleteProgram(_programID);
}

void OpenGLShader::bind() const {
  glUseProgram(_programID);
}

void OpenGLShader::unbind() const {
  glUseProgram(0);
}

void OpenGLShader::setMat4(std::string_view name, const Mat4& mat) {
  GLint location = glGetUniformLocation(_programID, name.data());
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

int OpenGLShader::totalTextureSlots() const {
  GLint textureUnits;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
  return static_cast<int>(textureUnits);
}

std::string OpenGLShader::preprocessShader(GLuint shaderType,
                                           std::string_view src) const {
  using namespace fmt::literals;
  if (shaderType == GL_VERTEX_SHADER) {
    return fmt::vformat(
        src, fmt::make_format_args("version"_a = "#version 330 core"));
  } else {
    int textureUnits = totalTextureSlots();
    Logger::info("Number of texture units available: {}", textureUnits);

    std::string switchOutputColorStr = "switch (id) {\n";
    for (int i = 0; i < textureUnits; i++) {
      switchOutputColorStr += fmt::format(
          "case {0}:\n"
          "o_Color = texture(u_Textures[{0}], v_TexCoord) * v_Color;\n"
          "break;\n",
          i);
    }
    switchOutputColorStr += "}";

    return fmt::vformat(
        src,
        fmt::make_format_args("version"_a = "#version 330 core",
                              "totalTextureSlot"_a = textureUnits,
                              "switchOutputColor"_a = switchOutputColorStr));
  }
}

std::optional<GLuint>
OpenGLShader::buildShaderProgram(std::string_view vertexSrc,
                                 std::string_view fragmentSrc) const {
  auto vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
  auto fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

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
      assert(false && "OpenGLShader linking failure!");

      return {};
    }

    glDetachShader(programID, vertexShader.value());
    glDetachShader(programID, fragmentShader.value());

    return programID;
  }

  return {};
}

std::optional<GLuint>
OpenGLShader::compileShader(GLuint shaderType, std::string_view rawSrc) const {
  GLuint shader = glCreateShader(shaderType);

  std::string src = preprocessShader(shaderType, rawSrc);
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
    assert(false && "OpenGLShader compilation failure!");
    glDeleteShader(shader);

    return {};
  }

  return shader;
}
} // namespace Engine::Renderer::Shader