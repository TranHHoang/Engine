#include <stb/stb_image.h>

#include <libcore/lib/Assert.hh>
#include <libcore/lib/Logger.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Texture.hh>

namespace Engine::Renderer::Texture {
void Texture::setImage(std::string_view path) {
  _path = path;

  int width, height, channels;
  stbi_uc* data =
      stbi_load(path.data(), &width, &height, &channels, STBI_rgb_alpha);
  assert(data);

  Logger::info("Loaded '{}'({}x{}, channels = {})",
               path.data(),
               width,
               height,
               channels);

  _width = width;
  _height = height;

  initTexture(data);
  stbi_image_free(data);
}

Region::Region()
    : _coords{{{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}} {
}

Region::Region(Ref<Texture> texture, const Vec2& coords, const Vec2& size) {
  Vec2 regionSize = size;
  if (size.x == 0 || size.y == 0) {
    regionSize.x = static_cast<float>(texture->width());
    regionSize.y = static_cast<float>(texture->height());
  }
  _coords = {{
      {(coords.x * regionSize.x) / texture->width(),
       (coords.y * regionSize.y) / texture->height()},

      {((coords.x + 1) * regionSize.x) / texture->width(),
       (coords.y * regionSize.y) / texture->height()},

      {((coords.x + 1) * regionSize.x) / texture->width(),
       ((coords.y + 1) * regionSize.y) / texture->height()},

      {(coords.x * regionSize.x) / texture->width(),
       ((coords.y + 1) * regionSize.y) / texture->height()},
  }};
}
} // namespace Engine::Renderer::Texture