#pragma once
#include <cstdint>
#include <span>

#include <libcore/Resource.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/PlatformProvider.hh>
#include <libcore/renderer/RawRenderer.hh>
#include <libcore/renderer/Shader.hh>
#include <libcore/renderer/Target.hh>
#include <libcore/renderer/Texture.hh>
#include <libcore/renderer/UniformLayout.hh>

namespace Engine::Renderer {
class Factory {
public:
  virtual ~Factory() = default;

  virtual Ref<Context>
  createContext(const PlatformProvider& provider) const = 0;
  virtual Unique<VertexBuffer>
  createVertexBuffer(size_t size, const BufferLayout& layout) const = 0;
  virtual Unique<IndexBuffer>
  createIndexBuffer(std::span<uint32_t> data) const = 0;
  virtual Unique<Shader> createShader(const UniformLayout& layout) const = 0;
  virtual Ref<Texture> createTexture(ResourceID resourceID,
                                     const Ref<Context>& context,
                                     uint32_t width,
                                     uint32_t height,
                                     const Texture::Info& info = {}) const = 0;
  virtual Unique<RawRenderer> createRawRenderer() const = 0;
  virtual Unique<Target> createTarget(const Ref<Context>& context,
                                      const Target::Info& info) const = 0;
  virtual Ref<Target> createDefaultTarget(const Target::Info& info) const = 0;
};
} // namespace Engine::Renderer