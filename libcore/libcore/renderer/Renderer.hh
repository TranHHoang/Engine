#pragma once
#include <cstdint>

#include <libcore/lib/Math.hh>
#include <libcore/lib/Memory.hh>
#include <libcore/renderer/Camera.hh>
#include <libcore/renderer/Context.hh>
#include <libcore/renderer/Factory.hh>
#include <libcore/renderer/RawRenderer.hh>
#include <libcore/renderer/Target.hh>
#include <libcore/renderer/Texture.hh>

namespace Engine::Renderer {
struct QuadVertex {
  Vec3 position;
  Vec4 color;
  Vec2 texCoord;
  float texIndex;
};

static const uint32_t MaxQuads = 1;
static constexpr const uint32_t QuadVertexCount = 4;
static constexpr const uint32_t MaxQuadVertices = MaxQuads * QuadVertexCount;
static constexpr const uint32_t MaxQuadIndices = MaxQuads * 6;
static const Vec4 QuadVertexOriginPositions[]{
    //  x     y     z    w
    {-0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.0f, 1.0f},
};
static const uint32_t WhiteTextureData = 0xFFFFFFFF;

class Renderer {
public:
  Renderer(const Factory& factory, const PlatformProvider& provider);
  void destroy();
  void setTarget(const Ref<Target>& target);
  void beginScene(const Camera& camera, const Mat4& cameraTransform);
  void endScene();
  void setClearColor(const Vec4& color);
  void drawQuad(const Mat4& transform,
                const Texture::Region* const textureRegion,
                const Vec4& color);

private:
  void beginBatch();
  void endBatch();
  void switchBatch();

private:
  Ref<Context> _context;
  Ref<Texture::Texture> _whiteTexture;
  Unique<RawRenderer> _rawRenderer;
  Vector<Texture::Texture*> _boundTextures;
  Vector<QuadVertex> _quadVertexData;
  uint32_t _boundTexturesCount;
  int _totalTextureSlots;
};
} // namespace Engine::Renderer