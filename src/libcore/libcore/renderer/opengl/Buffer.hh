#pragma once
#include <span>

#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer::OpenGL {
class VertexBuffer : public Engine::Renderer::VertexBuffer {
public:
  VertexBuffer(size_t size, const BufferLayout& layout);
  ~VertexBuffer();
  void setData(MemBlock data) override;

  GLuint bufferID() const { return _bufID; }

private:
  GLuint _bufID;
};

class IndexBuffer : public Engine::Renderer::IndexBuffer {
public:
  IndexBuffer(std::span<const uint32_t> indicies);
  ~IndexBuffer();

  GLuint bufferID() const { return _bufID; }

private:
  GLuint _bufID;
};
} // namespace Engine::Renderer::OpenGL