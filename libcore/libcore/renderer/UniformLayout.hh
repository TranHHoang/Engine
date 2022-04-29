#pragma once
#include <cstdint>
#include <initializer_list>
#include <string>

#include <libcore/lib/Vector.hh>
#include <libcore/renderer/BufferLayout.hh>

namespace Engine::Renderer::Shader {
enum class UniformType {
  // Follow OpenGL and Vulkan naming convention
  UniformBufferObject,
  CombinedImageSampler
};

enum class ShaderStage { Vertex, Fragment };

struct UniformElement {
  std::string name;
  UniformType type;
  Buffer::Layout layout;
  ShaderStage stage;
  uint32_t count;
  uint32_t offset;
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
} // namespace Engine::Renderer::Shader