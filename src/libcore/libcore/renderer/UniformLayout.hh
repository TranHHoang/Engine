#pragma once
#include <cstdint>
#include <initializer_list>
#include <string>

#include <libcore/lib/Vector.hh>
#include <libcore/renderer/BufferLayout.hh>

namespace Engine::Renderer {
struct UniformElement {
  enum class Type {
    // Follow OpenGL and Vulkan naming convention
    UniformBufferObject,
    CombinedImageSampler
  };

  enum class Stage { Vertex, Fragment };

  std::string name;
  Type type;
  BufferLayout layout;
  Stage stage;
  uint32_t count;
  uint32_t offset = 0;
};

class UniformLayout {
public:
  UniformLayout() = default;
  UniformLayout(const std::initializer_list<UniformElement>& elements)
      : _elements{elements} {
    uint32_t offset = 0;
    for (auto& e : _elements) {
      e.offset = offset;
      offset += e.count;
    }
    _stride = offset;
  }

  const Vector<UniformElement>& elements() const { return _elements; }
  uint32_t stride() const { return _stride; }

private:
  Vector<UniformElement> _elements;
  uint32_t _stride;
};
} // namespace Engine::Renderer