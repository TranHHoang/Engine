#pragma once
#include <libcore/lib/Memory.hh>
#include <libcore/lib/Vector.hh>
#include <libcore/renderer/BufferLayout.hh>

namespace Engine::Renderer {
class VertexBuffer {
public:
  VertexBuffer(size_t size, const BufferLayout& layout)
      : _size{size},
        _layout{layout} {
    setupLayout();
  }
  virtual ~VertexBuffer() = default;

  const BufferLayout& layout() const { return _layout; }

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void setData(MemBlock data) = 0;

protected:
  virtual void setupLayout() {}

protected:
  size_t _size;
  BufferLayout _layout;
};

class IndexBuffer {
public:
  IndexBuffer(size_t size) : _size{size} {}
  virtual ~IndexBuffer() = default;

  size_t size() const { return _size; }

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

private:
  size_t _size;
};
} // namespace Engine::Renderer