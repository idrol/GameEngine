#include <Engine/Framework/Components/MeshComponents/QuadMeshComponent.h>
#include <Engine/RenderEngine/Mesh/MeshFactory.h>
#include <Engine/RenderEngine/Material/PBRMaterial.h>

using namespace Engine::Framework;
using namespace Engine::RenderEngine;

QuadMeshComponent::QuadMeshComponent(float size) : MeshComponent(MeshFactory::CreateQuadMesh(std::make_shared<PBRMaterial>(), size)) {

}