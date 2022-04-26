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

namespace Engine::Renderer {
class OpenGLFactory : public Factory {
public:
  Ref<Context> createContext(const PlatformProvider& provider) const override {
    assert(dynamic_cast<const OpenGLPlatformProvider*>(&provider));
    return createRef<OpenGLContext>(
        static_cast<const OpenGLPlatformProvider&>(provider));
  }

  Unique<Buffer::Vertex>
  createVertexBuffer(size_t size, const Buffer::Layout& layout) const override {
    return createUnique<Buffer::OpenGLVertex>(size, layout);
  }

  Unique<Buffer::Index> createIndexBuffer(size_t size,
                                          uint32_t* data) const override {
    return createUnique<Buffer::OpenGLIndex>(size, data);
  }

  Unique<Shader::Shader>
  createShader(const Shader::UniformLayout& layout) const override {
    return createUnique<Shader::OpenGLShader>(layout);
  }

  Ref<Texture::Texture>
  createTexture(ResourceID resourceID,
                const Ref<Context>&,
                uint32_t width,
                uint32_t height,
                const Texture::Info& info) const override {
    return createRef<Texture::OpenGLTexture>(resourceID, width, height, info);
  }

  Unique<RawRenderer> createRawRenderer() const override {
    return createUnique<OpenGLRawRenderer>();
  }

  Unique<Target> createTarget(const Ref<Context>&,
                              const Target::Info& info) const override {
    return createUnique<OpenGLTarget>(info);
  }

  Ref<Target> createDefaultTarget(const Target::Info& info) const override {
    return createRef<OpenGLDefaultTarget>(info);
  }
};
} // namespace Engine::Renderer