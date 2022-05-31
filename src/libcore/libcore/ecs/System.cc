#include <libcore/ecs/Component.hh>
#include <libcore/ecs/Scene.hh>
#include <libcore/ecs/System.hh>

namespace Engine::ECS::System {
using namespace ECS::Component;

Rendering::Rendering(Renderer::Renderer& renderer,
                     const Ref<Renderer::Target>& target)
    : _renderer{renderer} {
  setTarget(target);
}

void Rendering::setTarget(const Ref<Renderer::Target>& target) {
  _renderer.setTarget(target);
}

void Rendering::onUpdate(Scene& scene, float) {
  const auto& [camera, transform] =
      scene.getComponent<Camera, Transform>(scene.mainCamera());

  _renderer.setClearColor(camera.backgroundColor);
  _renderer.beginScene(camera, transform);

  scene.forEach<Transform, Sprite>(
      [this](Transform& transform, Sprite& sprite) {
        _renderer.drawQuad(transform, &sprite.textureRegion, sprite.color);
      });

  _renderer.endScene();
}

void Spin::onUpdate(Scene& scene, float) {
  scene.forEach<Transform, Sprite>([](Transform& transform, Sprite&) {
    transform.rotation++; /* */
  });
}
} // namespace Engine::ECS::System