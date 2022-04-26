#include <algorithm>
#include <iterator>
#include <span>

#include <libcore/ContentManager.hh>
#include <libcore/lib/ListUtils.hh>
#include <libcore/renderer/Buffer.hh>
#include <libcore/renderer/BufferLayout.hh>
#include <libcore/renderer/Renderer.hh>
#include <libcore/renderer/UniformLayout.hh>

namespace Engine::Renderer {
Renderer::Renderer(const Factory& factory, const PlatformProvider& provider) {
  Logger::info("Initiating renderer context");
  _context = factory.createContext(provider);
  Logger::info("Initiating raw renderer (low level graphic API)");
  _rawRenderer = factory.createRawRenderer();

  Logger::info("Initiating vertex buffer");
  auto vertexBuf =
      factory.createVertexBuffer(MaxQuadVertices * sizeof(QuadVertex),
                                 {
                                     {"a_Position", Shader::DataType::Float3},
                                     {"a_Color", Shader::DataType::Float4},
                                     {"a_TexCoord", Shader::DataType::Float2},
                                     {"a_TexIndex", Shader::DataType::Float},
                                 });

  Logger::info("Initiating index buffer");
  auto quadIndices = createUnique<uint32_t[]>(MaxQuadIndices);
  std::array pattern{0, 1, 2, 2, 3, 0};
  for (uint32_t i = 0; i < MaxQuadIndices; i++) {
    quadIndices[i] = i / static_cast<uint32_t>(pattern.size()) * 4 +
                     pattern[i % pattern.size()];
  }
  auto indexBuf = factory.createIndexBuffer(MaxQuadIndices, quadIndices.get());

  Logger::info("Initiating shader");
  auto shader = factory.createShader({
      {
          .name = "u_ProjectionView",
          .type = Shader::UniformType::UniformBufferObject,
          .layout =
              {
                  {"projectionView", Shader::DataType::Mat4},
              },
          .stage = Shader::ShaderStage::Vertex,
          .count = 1,
      },
      {
          .name = "u_Textures",
          .type = Shader::UniformType::CombinedImageSampler,
          .stage = Shader::ShaderStage::Fragment,
          .count = 16,
      },
  });

  _rawRenderer->setData(
      std::move(vertexBuf), std::move(indexBuf), std::move(shader));

  Logger::info("Binding all texture slots with a default white texture");
  auto texture =
      ContentManager<Texture::Texture>::create(factory, _context, 1, 1);
  texture->initTexture(&WhiteTexture);
  std::fill(_boundTextures.begin(), _boundTextures.end(), texture);

  _quadVertexBase = createUnique<QuadVertex[]>(MaxQuadVertices);
  Logger::info("Renderer initialization completed");
}

void Renderer::destroy() {
  _context->destroy();
}

void Renderer::setTarget(const Ref<Target>& target) {
  _rawRenderer->setTarget(target);
}

void Renderer::beginScene(const Camera& camera, const Mat4& cameraTransform) {
  _rawRenderer->prepareScene();
  _rawRenderer->beginScene();
  _rawRenderer->uploadShaderUniforms({
      {"u_ProjectionView", camera.projection() * glm::inverse(cameraTransform)},
  });
  beginBatch();
}

void Renderer::beginBatch() {
  // Reset
  _quadVertexPtr = _quadVertexBase.get();
  _quadIndicesCount = 0;
  _boundTexturesCount = 1;
}

void Renderer::endBatch() {
  if (_quadIndicesCount == 0)
    return;

  auto batchSize = std::distance(_quadVertexBase.get(), _quadVertexPtr);
  _rawRenderer->setVertexBufferData(batchSize * sizeof(QuadVertex),
                                    _quadVertexBase.get());
  _rawRenderer->bindTextures(_boundTextures);
  _rawRenderer->drawIndexed(_quadIndicesCount);
}

void Renderer::endScene() {
  endBatch();
  _rawRenderer->endScene();
  _rawRenderer->destroyScene();
}

void Renderer::switchBatch() {
  endBatch();
  beginBatch();
}

void Renderer::setClearColor(const Vec4& color) {
  _rawRenderer->setClearColor(color);
}

void Renderer::drawQuad(const Mat4& transform,
                        const Texture::Region* const textureRegion,
                        const Vec4& color) {
  // Submit a draw call
  if (_quadIndicesCount >= MaxQuadIndices)
    switchBatch();

  const auto& region = textureRegion ? *textureRegion : Texture::Region{};
  auto textureCoords = region.coords();

  float textureIndex = 0.0f;
  if (region.texture()) {
    auto foundIndex = ListUtils::findIndex(
        std::span{_boundTextures.begin(), _boundTexturesCount},
        [&region](const Ref<Texture::Texture>& tex) {
          return tex->resourceID() == region.texture()->resourceID();
        });

    if (foundIndex == -1) {
      if (_boundTexturesCount >= MaxTextureSlots)
        switchBatch();

      textureIndex = static_cast<float>(_boundTexturesCount);
      _boundTextures[_boundTexturesCount++] = region.texture();
    } else {
      textureIndex = static_cast<float>(foundIndex);
    }
  }

  for (uint32_t i = 0; i < QuadVertexCount; ++i) {
    _quadVertexPtr->position = transform * QuadVertexOriginPositions[i];
    _quadVertexPtr->color = color;
    _quadVertexPtr->texCoord = textureCoords[i];
    _quadVertexPtr->texIndex = textureIndex;
    ++_quadVertexPtr;
  }
  _quadIndicesCount += 6;
}
} // namespace Engine::Renderer
