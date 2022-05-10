#pragma once
#include <span>

#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::Buffer {
class OpenGLVertex : public Vertex {
public:
  OpenGLVertex(size_t size, const Layout& layout);
  ~OpenGLVertex();

  void bind() const override;
  void unbind() const override;
  void setData(MemBlock data) override;

private:
  GLuint _bufID;
};

class OpenGLIndex : public Index {
public:
  OpenGLIndex(std::span<const uint32_t> indicies);
  ~OpenGLIndex();

  virtual void bind() const override;
  virtual void unbind() const override;

private:
  GLuint _bufID;
};
} // namespace Engine::Renderer::Buffer