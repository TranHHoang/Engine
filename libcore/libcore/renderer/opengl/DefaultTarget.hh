#pragma once
#include <glad/gl.h>

#include <libcore/renderer/Target.hh>

namespace Engine::Renderer {
class OpenGLDefaultTarget : public Target {
public:
  OpenGLDefaultTarget(const Info& info) : Target{info} {}
  void bind() override {}
  void unbind() override {}
  void invalidate() override { glViewport(0, 0, _info.width, _info.height); }
};
} // namespace Engine::Renderer