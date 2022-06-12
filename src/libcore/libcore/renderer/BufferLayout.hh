#pragma once
#include <string>
#include <string_view>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/lib/Vector.hh>

namespace Engine::Renderer {
struct BufferElement {
  enum class Type {
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

  std::string name;
  Type type;
  size_t offset;

  BufferElement() = default;
  BufferElement(std::string_view name, Type type)
      : name{name},
        type{type},
        offset{0} {}

  inline constexpr int componentCount() const {
    switch (type) {
    case Type::Float:
    case Type::Int:
    case Type::Bool:
      return 1;
    case Type::Float2:
    case Type::Int2:
      return 2;
    case Type::Float3:
    case Type::Mat3: // 3 x float3
    case Type::Int3:
      return 3;
    case Type::Float4:
    case Type::Mat4: // 4 x float4
    case Type::Int4:
      return 4;
    default:
      assert(false);
      return 0;
    }
  }

  inline constexpr int size() const {
    switch (type) {
    case Type::Bool:
      return 1;
    case Type::Float:
    case Type::Int:
      return 4;
    case Type::Float2:
    case Type::Int2:
      return 4 * 2;
    case Type::Float3:
    case Type::Int3:
      return 4 * 3;
    case Type::Float4:
    case Type::Int4:
      return 4 * 4;
    case Type::Mat3:
      return 4 * 3 * 3;
    case Type::Mat4:
      return 4 * 4 * 4;
    default:
      assert(false);
      return 0;
    }
  }
};

class BufferLayout {
public:
  BufferLayout() = default;
  BufferLayout(const std::initializer_list<BufferElement>& elements)
      : _elements{elements} {
    int offset = 0;
    for (BufferElement& e : _elements) {
      e.offset = offset;
      offset += e.size();
    }
    _stride = offset;
  }

  int stride() const { return _stride; }
  const Vector<BufferElement>& elements() const { return _elements; }

private:
  Vector<BufferElement> _elements;
  int _stride;
};
} // namespace Engine::Renderer