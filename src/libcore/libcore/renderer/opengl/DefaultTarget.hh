#pragma once
#include <libcore/renderer/Target.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer {
class OpenGLDefaultTarget : public Target {
public:
  OpenGLDefaultTarget(const Info& info) : Target{info} {}
  void bind() override {}
  void unbind() override {}
  void invalidate() override { glViewport(0, 0, _info.width, _info.height); }
};
} // namespace Engine::Renderer