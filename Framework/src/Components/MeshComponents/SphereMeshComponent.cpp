#include <Engine/Framework/Components/MeshComponents/SphereMeshComponent.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include <Engine/RenderEngine/Material/PBRMaterial.h>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;

SphereMeshComponent::SphereMeshComponent(float radius, unsigned int rings, unsigned int sectors): MeshComponent(MeshFactory::CreateSphereMesh(std::make_shared<PBRMaterial>(), radius, rings, sectors)) {

}