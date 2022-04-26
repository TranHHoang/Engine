#include <glm/gtc/matrix_transform.hpp>

#include <libcore/renderer/Camera.hh>

namespace Engine::Renderer {
Camera::Camera()
    : _size{1.0f},
      _near{-1.0f},
      _far{1.0f},
      _aspectRatio{16.0f / 9.0f} {
  calculateProjection();
}

const Mat4& Camera::projection() const {
  return _proj;
}

void Camera::setProjection(float aspectRatio, float size) {
  _aspectRatio = aspectRatio;
  _size = size;
  calculateProjection();
}

void Camera::calculateProjection() {
  float left = -_size * _aspectRatio * 0.5f;
  float right = -left;
  float top = _size * 0.5f;
  float bottom = -top;

  _proj = glm::ortho(left, right, bottom, top, _near, _far);
}
} // namespace Engine::Renderer