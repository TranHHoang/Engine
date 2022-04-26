#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <string_view>

#include <libcore/Resource.hh>
#include <libcore/lib/Math.hh>
#include <libcore/lib/Memory.hh>

namespace Engine::Renderer::Texture {
struct Info {
  enum class AddressMode { Repeat };
  enum class Filter { Linear, Nearest };

  AddressMode u, v;
  Filter min, mag;

  Info(AddressMode u = AddressMode::Repeat,
       AddressMode v = AddressMode::Repeat,
       Filter min = Filter::Linear,
       Filter mag = Filter::Linear)
      : u{u},
        v{v},
        min{min},
        mag{mag} {}
};

class Texture {
public:
  Texture(ResourceID resourceID,
          uint32_t width,
          uint32_t height,
          const Info& info)
      : _info{info},
        _width{width},
        _height{height},
        _resourceID{resourceID} {}
  virtual ~Texture() = default;

  ResourceID resourceID() const { return _resourceID; }
  uint32_t width() const { return _width; }
  uint32_t height() const { return _height; }
  void setImage(std::string_view path);

  virtual void initTexture(const void* data) = 0;

protected:
  Info _info;
  uint32_t _width, _height;
  std::string _path;

private:
  ResourceID _resourceID;
};

class Region {
public:
  Region();
  Region(Ref<Texture> texture, const Vec2& coords = {}, const Vec2& size = {});

  const Ref<Texture>& texture() const { return _texture; }
  const std::array<Vec2, 4>& coords() const { return _coords; }

private:
  Ref<Texture> _texture;
  std::array<Vec2, 4> _coords;
};
} // namespace Engine::Renderer::Texture