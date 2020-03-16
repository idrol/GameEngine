#include <Engine/RenderEngine/Mesh/ProceduralSkybox.h>
#include <Engine/RenderEngine/Material/ProcSkyboxMaterial.h>

using namespace Engine::RenderEngine;

ProceduralSkybox::ProceduralSkybox(std::shared_ptr<Light> light): Skybox(std::make_shared<ProcSkyboxMaterial>()) {
	auto mat = std::dynamic_pointer_cast<ProcSkyboxMaterial>(material);
	mat->SetSun(light);
}