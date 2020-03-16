#include "BRDFUtil.h"
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Renderer/RenderState.h>
#include <Engine/RenderEngine/Shader/ShaderCache.h>
#include <Engine/RenderEngine/Mesh/Mesh.h>
#include "Shader/BRDFShader.h"
#include "Framebuffer/TextureRenderFramebuffer.h"
#include <Engine/Library/Console.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>


using namespace Engine::RenderEngine;

std::shared_ptr<Texture2D> BRDFUtil::CreateMap() {
	auto texture = std::make_shared<Texture2D>(); // Allocates a handle
	int brdfSize = 512;
	texture->Bind();
	texture->SetFormat(brdfSize, brdfSize, GL_RG16F, GL_RG, GL_FLOAT, true);
	texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, true);
	texture->SetFiltering(GL_LINEAR, GL_LINEAR, true);

	auto lastActiveFramebuffer = RenderState::GetActiveFramebuffer();
	auto textureRenderFramebuffer = RenderEngine::GetTextureRenderFramebuffer();
	textureRenderFramebuffer->Resize(brdfSize, brdfSize);
	RenderState::SetActiveFramebuffer(textureRenderFramebuffer);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	textureRenderFramebuffer->BindTexture(texture->GetTextureTarget());
	auto brdfShader = std::dynamic_pointer_cast<BRDFShader>(ShaderCache::GetCachedShader("brdfShader"));
	brdfShader->Use();
	MeshFactory::CreateFullScreenQuadMesh(nullptr)->RenderGeometry();

	// Restore state
	RenderState::SetActiveFramebuffer(lastActiveFramebuffer);
	return texture;
}