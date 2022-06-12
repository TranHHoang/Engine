#pragma once
#include <glm/gtc/matrix_transform.hpp>

#include <libcore/lib/Math.hh>
#include <libcore/renderer/Camera.hh>
#include <libcore/renderer/Texture.hh>

namespace Engine::ECS::Component {
struct Transform {
  Vec2 translation{0.0f, 0.0f};
  float rotation{0.0f};
  Vec2 scale{1.0f, 1.0f};

  operator Mat4() const { return transformMatrix(); }

  Mat4 transformMatrix() const {
    return glm::translate(glm::mat4(1.0f), {translation, 0.0f}) *
           glm::rotate(
               glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1)) *
           glm::scale(glm::mat4(1.0f), {scale, 1.0f});
  }
};

struct Camera {
  Renderer::Camera camera;
  Vec4 backgroundColor{0.0f, 1.0f, 0.0f, 1.0f};

  operator Renderer::Camera() const { return camera; }
};

struct Sprite {
  Vec4 color{1.0f, 0.0f, 0.0f, 1.0f};
  Renderer::TextureRegion textureRegion{};
};
} // namespace Engine::ECS::Component