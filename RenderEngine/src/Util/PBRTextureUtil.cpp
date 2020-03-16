#include <Engine/RenderEngine/PBR/PBRTextureUtil.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include <Engine/RenderEngine/Mesh/Skybox.h>
#include <Engine/RenderEngine/Material/CubemapMaterial.h>
#include "Material/PrefilterMaterial.h"
#include "Framebuffer/TextureRenderFramebuffer.h"
#include "TextureRenderer.h"

using namespace Engine::RenderEngine;
using namespace Engine::Library;

std::shared_ptr<Mesh> PBRTextureUtil::irradianceMesh;
std::shared_ptr<Mesh> PBRTextureUtil::prefilterMesh;

Transform* irradianceTrans;
Transform* prefilterTrans;

void PBRTextureUtil::CreateMeshes() {
	irradianceMesh = MeshFactory::CreateInternalCubemapMesh(std::make_shared<CubemapMaterial>("irradianceShader"));
	irradianceTrans = new Transform();
	irradianceTrans->position = glm::vec3(-1);
	irradianceTrans->scale = glm::vec3(2);
	irradianceMesh->SetTransform(irradianceTrans);
	
	prefilterMesh = MeshFactory::CreateInternalCubemapMesh(std::make_shared<PrefilterMaterial>());
	prefilterTrans = new Transform();
	prefilterTrans->position = glm::vec3(-1);
	prefilterTrans->scale = glm::vec3(2);
	prefilterMesh->SetTransform(prefilterTrans);
}

void PBRTextureUtil::DestroyMeshes() {
	irradianceMesh = nullptr;
	prefilterMesh = nullptr;

	delete irradianceTrans;
	delete prefilterTrans;
}

Environment PBRTextureUtil::CreateEnvironment(std::shared_ptr<Skybox> skybox) {
	auto environmentCubemap = CreateEnvironmentMap(skybox);
	auto irradiance = CreateIrradianceMap(environmentCubemap);
	auto prefilter = CreatePrefilterMap(environmentCubemap);
	return Environment(prefilter, irradiance);
}

std::shared_ptr<TextureCubemap> PBRTextureUtil::CreateEnvironmentMap(std::shared_ptr<Skybox> skybox) {
	auto cubemap = std::make_shared<TextureCubemap>();
	cubemap->SetFormat(258, 258, GL_RGB16F, GL_RGB, GL_FLOAT);
	cubemap->UseBilinearFiltering();
	cubemap->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	TextureRenderer::RenderCubemap(cubemap->GetWidth(), cubemap, skybox);
	return cubemap;
}

std::shared_ptr<TextureCubemap> PBRTextureUtil::CreateIrradianceMap(std::shared_ptr<TextureCubemap> cubemap) {
	const int irradianceSize = 32;
	auto irradianceCubemap = std::make_shared<TextureCubemap>();
	irradianceCubemap->SetFormat(irradianceSize, irradianceSize, GL_RGB16F, GL_RGB, GL_FLOAT);
	irradianceCubemap->UseBilinearFiltering();
	irradianceCubemap->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	
	auto cubemapMat = std::dynamic_pointer_cast<CubemapMaterial>(irradianceMesh->GetMaterial());
	cubemapMat->SetCubemap(cubemap);

	TextureRenderer::RenderCubemap(irradianceSize, irradianceCubemap, irradianceMesh);
	return irradianceCubemap;
}

std::shared_ptr<TextureCubemap> PBRTextureUtil::CreatePrefilterMap(std::shared_ptr<TextureCubemap> cubemap) {
	const int prefilterSize = 128;
	std::shared_ptr<TextureCubemap> prefilterCubemap = std::make_shared<TextureCubemap>();
	prefilterCubemap->SetFormat(prefilterSize, prefilterSize, GL_RGB16F, GL_RGB, GL_FLOAT);
	prefilterCubemap->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	prefilterCubemap->SetFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	prefilterCubemap->GenerateMipMaps();

	auto cubemapMat = std::dynamic_pointer_cast<PrefilterMaterial>(prefilterMesh->GetMaterial());
	cubemapMat->SetCubemap(cubemap);

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
		int mipSize = (int)(prefilterSize * std::pow(0.5, mip));

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		cubemapMat->SetRoughness(roughness);
		TextureRenderer::RenderCubemap(mipSize, prefilterCubemap, prefilterMesh, mip);
	}

	return prefilterCubemap;
}