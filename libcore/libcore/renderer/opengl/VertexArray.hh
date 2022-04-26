#pragma once
#include <cstdint>

#include <libcore/renderer/Buffer.hh>

namespace Engine::Renderer {
class OpenGLVertexArray {
public:
  OpenGLVertexArray();
  void bind() const;
  void unbind() const;
  void setBuffers(const Buffer::Vertex& vertexBuf,
                  const Buffer::Index& indexBuf);

private:
  uint32_t _arrayID;
};
} // namespace Engine::Renderer