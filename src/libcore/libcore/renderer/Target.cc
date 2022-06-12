#include <libcore/renderer/Target.hh>

namespace Engine::Renderer {
void Target::resize(uint32_t width, uint32_t height) {
  if (width == _info.width && height == _info.height)
    return;

  Logger::info("Target is resized to {}x{}", width, height);
  if (width == 0 || height == 0) {
    Logger::error("Invalid target size {}x{}", width, height);
    return;
  }

  _info = {width, height};
  invalidate();
}
} // namespace Engine::Renderer