#pragma once
#include <libcore/lib/Memory.hh>
#include <libcore/lib/Vector.hh>
#include <libcore/renderer/BufferLayout.hh>

namespace Engine::Renderer::Buffer {
class Vertex {
public:
  Vertex(size_t size, const Layout& layout) : _size{size}, _layout{layout} {
    setupLayout();
  }
  virtual ~Vertex() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void setData(size_t size, const void* data) = 0;

  const Layout& layout() const { return _layout; }

protected:
  virtual void setupLayout() {}

protected:
  size_t _size;
  Layout _layout;
};

class Index {
public:
  Index(size_t size) : _size{size} {}
  virtual ~Index() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  size_t size() const { return _size; }

private:
  size_t _size;
};
} // namespace Engine::Renderer::Buffer