#pragma once
#include <cstdint>

#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/opengl/Glad.hh>

namespace Engine::Renderer {
class OpenGLVertexArray {
public:
  OpenGLVertexArray();
  void bind() const;
  void unbind() const;
  void setBuffers(const Buffer::Vertex& vertexBuf,
                  const Buffer::Index& indexBuf);

private:
  GLuint _arrayID;
};
} // namespace Engine::Renderer