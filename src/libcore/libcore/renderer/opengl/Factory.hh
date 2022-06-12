#pragma once
#include <libcore/lib/Assert.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Factory.hh>
#include <libcore/renderer/opengl/Buffer.hh>
#include <libcore/renderer/opengl/Context.hh>
#include <libcore/renderer/opengl/DefaultTarget.hh>
#include <libcore/renderer/opengl/PlatformProvider.hh>
#include <libcore/renderer/opengl/RawRenderer.hh>
#include <libcore/renderer/opengl/Shader.hh>
#include <libcore/renderer/opengl/Target.hh>
#include <libcore/renderer/opengl/Texture.hh>

namespace Engine::Renderer::OpenGL {
namespace Common = Engine::Renderer;

class Factory : public Common::Factory {
public:
  Ref<Common::Context>
  createContext(const Common::PlatformProvider& provider) const override {
    assert(dynamic_cast<const PlatformProvider*>(&provider));
    return createRef<Context>(static_cast<const PlatformProvider&>(provider));
  }

  Unique<Common::VertexBuffer>
  createVertexBuffer(size_t size, const BufferLayout& layout) const override {
    return createUnique<VertexBuffer>(size, layout);
  }

  Unique<Common::IndexBuffer>
  createIndexBuffer(std::span<uint32_t> data) const override {
    return createUnique<IndexBuffer>(data);
  }

  Unique<Common::Shader>
  createShader(const UniformLayout& layout) const override {
    return createUnique<Shader>(layout);
  }

  Ref<Common::Texture> createTexture(ResourceID resourceID,
                                     const Ref<Common::Context>&,
                                     uint32_t width,
                                     uint32_t height,
                                     const Texture::Info& info) const override {
    return createRef<Texture>(resourceID, width, height, info);
  }

  Unique<Common::RawRenderer> createRawRenderer() const override {
    return createUnique<RawRenderer>();
  }

  Unique<Common::Target> createTarget(const Ref<Common::Context>&,
                                      const Target::Info& info) const override {
    return createUnique<Target>(info);
  }

  Ref<Common::Target>
  createDefaultTarget(const Target::Info& info) const override {
    return createRef<DefaultTarget>(info);
  }
};
} // namespace Engine::Renderer::OpenGL