#pragma once
#include <string>
#include <string_view>

#include <libcore/Resource.hh>
#include <libcore/lib/HashMap.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/Factory.hh>
#include <libcore/renderer/Texture.hh>

namespace Engine {
template <typename T>
class ContentManager {
public:
  template <typename... Args>
  static Ref<T> create(const Renderer::Factory& factory,
                       const Ref<Renderer::Context>& context,
                       Args&&... args);
  template <typename... Args>
  static Ref<T> load(const Renderer::Factory& factory,
                     const Ref<Renderer::Context>& context,
                     std::string_view path,
                     Args&&... args);

private:
  inline static HashMap<std::string, Ref<T>> _resources;
  inline static ResourceID _uuid{0};
};

template <>
template <typename... Args>
Ref<Renderer::Texture>
ContentManager<Renderer::Texture>::create(const Renderer::Factory& factory,
                                          const Ref<Renderer::Context>& context,
                                          Args&&... args) {
  std::string key = "#texture_" + std::to_string(_uuid);
  _resources[key] =
      factory.createTexture(_uuid, context, std::forward<Args>(args)...);
  _uuid++;
  return _resources[key];
}

template <>
template <typename... Args>
Ref<Renderer::Texture>
ContentManager<Renderer::Texture>::load(const Renderer::Factory& factory,
                                        const Ref<Renderer::Context>& context,
                                        std::string_view path,
                                        Args&&... args) {
  auto p = std::string{path};
  if (not _resources.contains(p)) {
    Ref<Renderer::Texture> texture =
        create(factory, context, std::forward<Args>(args)...);
    texture->setImage(path);
    _resources[p] = std::move(texture);
  }
  return _resources[p];
}
} // namespace Engine