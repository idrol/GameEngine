#include <Engine/RenderEngine/Util/EquirectangularTextureConverter.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Material/TexturedMaterial.h>
#include "Util/TextureRenderer.h"

using namespace Engine::RenderEngine;
using namespace Engine::Library;

std::shared_ptr<TextureCubemap> EquirectangularTextureConverter::Convert(std::shared_ptr<Texture2D> equirectangularTexture) {
    const int cubemapSize = 512;

    auto cubemapTexture = std::make_shared<TextureCubemap>();
    cubemapTexture->SetFormat(cubemapSize, cubemapSize, GL_RGB16F, GL_RGB, GL_FLOAT);
    cubemapTexture->UseBilinearFiltering();
    cubemapTexture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    auto cubemapMat = std::make_shared<TexturedMaterial>("equirectangularToCubemapShader");
    cubemapMat->SetTexture(equirectangularTexture);

    Transform* cubemapTrans = new Transform();
    auto cubemapMesh = MeshFactory::CreateInternalCubemapMesh(cubemapMat);
    cubemapTrans->position = glm::vec3(-1);
    cubemapTrans->scale = glm::vec3(2);
    cubemapMesh->SetTransform(cubemapTrans);

    TextureRenderer::RenderCubemap(cubemapSize, cubemapTexture, cubemapMesh);
    return cubemapTexture;
}