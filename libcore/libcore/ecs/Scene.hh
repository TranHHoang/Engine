#pragma once
#include <entt/entity/registry.hpp>
#include <libcore/ecs/Component.hh>

namespace Engine::ECS {
using Entity = entt::entity;

class Scene {
public:
  Scene() {
    _camera = createEntity();
    addComponent<Component::Camera>(_camera);
    addComponent<Component::Transform>(_camera);
  }

  Entity createEntity() { return _registry.create(); }
  void destroyEntity(Entity entity) { _registry.destroy(entity); }

  template <typename Ta, typename Tb, typename... Ty, typename Fn>
  void forEach(Fn fn) {
    _registry.group<Ta, Tb, Ty...>().each(fn);
  }

  template <typename T, typename Fn>
  void forEach(Fn fn) {
    _registry.view<T>().each(fn);
  }

  template <typename T, typename... Args>
  T& addComponent(const Entity entity, Args&&... args) {
    return _registry.emplace<T>(entity, std::forward<Args>(args)...);
  }

  template <typename... T>
  decltype(auto) getComponent(const Entity entity) {
    return _registry.get<T...>(entity);
  }

  template <typename T>
  bool hasComponent(const Entity entity) const {
    return _registry.try_get<T>(entity) != nullptr;
  }

  Entity mainCamera() const { return _camera; }

private:
  entt::registry _registry;
  Entity _camera;
};
} // namespace Engine::ECS