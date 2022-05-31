#pragma once
#include <libcore/ecs/Scene.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Renderer.hh>
#include <libcore/renderer/Target.hh>

namespace Engine::ECS::System {
class System {
public:
  virtual ~System() = default;
  virtual void onUpdate(Scene& scene, float deltaTime) = 0;
};

class Rendering : public System {
public:
  Rendering(Renderer::Renderer& renderer, const Ref<Renderer::Target>& target);
  void setTarget(const Ref<Renderer::Target>& target);
  void onUpdate(Scene& scene, float deltaTime) override;

private:
  Renderer::Renderer& _renderer;
};

class Spin : public System {
public:
  void onUpdate(Scene& scene, float deltaTime) override;
};
} // namespace Engine::ECS::System