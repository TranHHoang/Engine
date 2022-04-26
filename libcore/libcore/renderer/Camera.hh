#pragma once
#include <libcore/lib/Math.hh>

namespace Engine::Renderer {
class Camera {
public:
  Camera();
  const Mat4& projection() const;
  void setProjection(float aspectRatio, float size);

private:
  void calculateProjection();

private:
  Mat4 _proj;
  float _size, _near, _far;
  float _aspectRatio;
};
} // namespace Engine::Renderer