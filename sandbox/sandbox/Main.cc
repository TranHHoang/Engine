#include <libcore/Engine.hh>

using namespace Engine;

class Sandbox : public App {
public:
  Sandbox() : App{{}, Renderer::API::OpenGL} {
    // TODO: Thread-safe
    rec = _window->activeScene().createEntity();
    auto& transform =
        _window->activeScene().addComponent<ECS::Component::Transform>(rec);
    transform.scale = {.5f, .5f};
    _window->activeScene().addComponent<ECS::Component::Sprite>(rec);
    rec2 = _window->activeScene().createEntity();
    auto& transform2 =
        _window->activeScene().addComponent<ECS::Component::Transform>(rec2);
    transform2.scale = {.5f, .5f};
    transform2.translation.x = 0.5;
    auto& s = _window->activeScene().addComponent<ECS::Component::Sprite>(rec2);
    s.color = {1.0f, 1.0f, 0.0f, 0.5f};
  }

private:
  ECS::Entity rec, rec2;
};

App* createApp() {
  return new Sandbox();
}