#pragma once
#include <cstdint>
#include <initializer_list>
#include <span>
#include <string>
#include <variant>

#include <libcore/lib/Math.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/lib/Vector.hh>
#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/Shader.hh>
#include <libcore/renderer/Target.hh>
#include <libcore/renderer/Texture.hh>

namespace Engine::Renderer {
struct UniformType {
  std::string name;
  std::variant<Mat4> value;
};

class RawRenderer {
public:
  virtual ~RawRenderer() = default;

  void setData(Unique<VertexBuffer> vertexBuf,
               Unique<IndexBuffer> indexBuf,
               Unique<Shader> shader);
  void setTarget(const Ref<Target>& target);

  virtual void prepareScene() = 0;
  virtual void beginScene() = 0;
  virtual void endScene() = 0;
  virtual void destroyScene() = 0;
  virtual void setClearColor(const Vec4& color) = 0;
  virtual void bindTextures(const Vector<Texture*>& textures) = 0;
  virtual void setVertexBufferData(MemBlock data) = 0;
  virtual void
  uploadShaderUniforms(const std::initializer_list<UniformType>& uniforms) = 0;
  virtual void drawIndexed(uint32_t indexCount) = 0;

protected:
  virtual void setupBuffers() {}
  virtual void setupShader() {}

protected:
  Unique<VertexBuffer> _vertexBuf;
  Unique<IndexBuffer> _indexBuf;
  Unique<Shader> _shader;
  Ref<Target> _target;
};
} // namespace Engine::Renderer