#pragma once
#include <string>
#include <string_view>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/lib/Vector.hh>

namespace Engine::Renderer::Shader {
enum class DataType {
  None,
  Bool,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4
};

static int sizeOf(DataType type) {
  switch (type) {
  case DataType::Bool:
    return 1;
  case DataType::Float:
  case DataType::Int:
    return 4;
  case DataType::Float2:
  case DataType::Int2:
    return 4 * 2;
  case DataType::Float3:
  case DataType::Int3:
    return 4 * 3;
  case DataType::Float4:
  case DataType::Int4:
    return 4 * 4;
  case DataType::Mat3:
    return 4 * 3 * 3;
  case DataType::Mat4:
    return 4 * 4 * 4;
  default:
    assert(false);
    return 0;
  }
}
} // namespace Engine::Renderer::Shader

namespace Engine::Renderer::Buffer {
struct Element {
  std::string name;
  Shader::DataType type;
  int size;
  size_t offset;

  Element() = default;
  Element(std::string_view name, Shader::DataType type)
      : name{name},
        type{type},
        size{Shader::sizeOf(type)},
        offset{0} {}

  int componentCount() const {
    switch (type) {
    case Shader::DataType::Float:
    case Shader::DataType::Int:
    case Shader::DataType::Bool:
      return 1;
    case Shader::DataType::Float2:
    case Shader::DataType::Int2:
      return 2;
    case Shader::DataType::Float3:
    case Shader::DataType::Mat3: // 3 x float3
    case Shader::DataType::Int3:
      return 3;
    case Shader::DataType::Float4:
    case Shader::DataType::Mat4: // 4 x float4
    case Shader::DataType::Int4:
      return 4;
    default:
      assert(false);
      return 0;
    }
  }
};

class Layout {
public:
  Layout() = default;
  Layout(const std::initializer_list<Element>& elements) : _elements{elements} {
    int offset = 0;
    for (Element& e : _elements) {
      e.offset = offset;
      offset += e.size;
    }
    _stride = offset;
  }

  int stride() const { return _stride; }
  const Vector<Element>& elements() const { return _elements; }

private:
  Vector<Element> _elements;
  int _stride;
};
} // namespace Engine::Renderer::Buffer