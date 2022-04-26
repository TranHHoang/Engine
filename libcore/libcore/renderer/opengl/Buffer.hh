#pragma once
#include <glad/gl.h>

#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Buffer.hh>

namespace Engine::Renderer::Buffer {
class OpenGLVertex : public Vertex {
public:
  OpenGLVertex(size_t size, const Layout& layout);
  ~OpenGLVertex();

  void bind() const override;
  void unbind() const override;
  void setData(size_t size, const void* data) override;

private:
  GLuint _bufID;
};

class OpenGLIndex : public Index {
public:
  OpenGLIndex(size_t size, uint32_t* indices);
  ~OpenGLIndex();

  virtual void bind() const override;
  virtual void unbind() const override;

private:
  GLuint _bufID;
};
} // namespace Engine::Renderer::Buffer