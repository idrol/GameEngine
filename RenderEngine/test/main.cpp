//
// Created by idrol on 20/01/2020.
//

#include <iostream>
#include <Engine/Library/Console.h>
#include <Engine/Library/Math.h>
#include <Engine/Library/PerlinNoise.h>
#include <Engine/Debug.h>
#include <Engine/RenderEngine.h>
#include <Engine/RenderEngine/Mesh/ProceduralSkybox.h>
#include <Engine/PostProcessing.h>
#include <Engine/Input/InputManager.h>
#include <Engine/RenderEngine/Debug/CameraController.h>
#include <Engine/RenderEngine/Shader/ShaderUBOS.h>
#include <Engine/Debug.h>
#include <Engine/ImGui.h>
#include <Engine/MeshExtras.h>
#include <Engine/ImguiRenderEngine.h>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/rotate_vector.hpp>
#include <malloc.h>
#include <Engine/RenderEngine/Model/ModelLoader.h>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

std::shared_ptr<RenderPipeline> mainPipeline;
Transform *terrainTransform = nullptr; //testing lol

void CreateMaterialTestScene() {
    // Creates a grid with 11x11 spheres with different roughnes and metallic values
    for(int m = 0;m<=10;m++) {
        for(int r = 0; r<=10;r++) {
            float metallic = ((float)m)/10.0f;
            float roughness = ((float)r)/10.0f;

            glm::vec3 position = glm::vec3(r*2, m*2+20, 0);
            Transform* transform = new Transform();
            auto sphereMat = std::make_shared<PBRMaterial>();
            auto sphere = MeshFactory::CreateSphereMesh(sphereMat, 1, 60, 60);
            sphere->SetTransform(transform);
            sphereMat->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
            sphereMat->SetRoughness(roughness);
            sphereMat->SetMetallic(metallic);
            mainPipeline->AddMesh(sphere);
            transform->position = position;
        }
    }
}

void CreateSingleSphereTestScene() {
    std::shared_ptr<PBRMaterial> singleSphereMat = std::make_shared<PBRMaterial>();
    Transform* transform = new Transform();
    auto sphere = MeshFactory::CreateSphereMesh(singleSphereMat, 2, 120, 120);
    sphere->SetTransform(transform);
    singleSphereMat->SetAlbedo(glm::vec3(1.0f, 0.0f, 0.0f));
    singleSphereMat->SetRoughness(0.01f);
    singleSphereMat->SetMetallic(1.0f);
    mainPipeline->AddMesh(sphere);
    transform->position = glm::vec3(0, 20, -10);
}

void CreateTerrainCell(std::vector<Vertex>& vertices, float p1, float p2, float p3, float p4, int qualityLevel, float cellBaseSize, float offsetX, float offsetY) {
    float qualitySize = cellBaseSize / (float)qualityLevel;
    
    for (int y = 0; y < qualityLevel; y++) {
float verticalPercent = (y + 1) / qualityLevel;
float p4p2 = Math::lerp(p4, p2, verticalPercent);
float p3p1 = Math::lerp(p3, p1, verticalPercent);
for (int x = 0; x < qualityLevel; x++) {
    float horizontalPercent = (x + 1) / qualityLevel;

    // For percent 0 p1 should be chosen
    // For percent 1 p4 should be chosen
    // p4---p3
    // |     |
    // p2---p1


    float height = Math::lerp(p4p2, p3p1, horizontalPercent);

    vertices.emplace_back(
        x * qualitySize + offsetX, height, y * qualitySize + offsetY,
        0, 1, 0,
        0, 0
    );
}
    }
}

void CreateTerrainFaces(std::shared_ptr<Model> model, unsigned int rowSize) {
    for (int y = 0; y < rowSize - 1; y++) {
        for (int x = 0; x < rowSize - 1; x++) {
            int offset = y * rowSize + x;
            int offset2 = offset + rowSize;

            model->AddFace(Face(offset + 1, offset, offset2));
            model->AddFace(Face(offset + 1, offset2, offset2 + 1));
        }
    }
}

std::shared_ptr<Model> CreateTerrainModel(float* procData, int procDataSize, int level) {
    auto model = std::make_shared<Model>();
    const float cellBaseSize = 1.0f;
    for (int y = 0; y < procDataSize; y++) {
        for (int x = 0; x < procDataSize; x++) {
            int index = y * procDataSize + x;
            model->AddVertex(Vertex(
                x * cellBaseSize, procData[index], y * cellBaseSize,
                0, 1, 0,
                0, 0
            ));
        }
    }
    CreateTerrainFaces(model, procDataSize);
    model->CalculateNormals();
    return model;
}

void CreateTerrainTestScene() {
    siv::PerlinNoise noise = siv::PerlinNoise(5646456684);
    int terrainSize = 1024;
    int proceduralTerrain = terrainSize + 1;
    float* procTerrain = (float*)malloc(sizeof(float) * proceduralTerrain * proceduralTerrain);
    float largestheight = 0.0f, lowestHeight = 0.0f;
    for (int y = 0; y < proceduralTerrain; y++) {
        for (int x = 0; x < proceduralTerrain; x++) {
            int baseIndex = (y * proceduralTerrain) + x;
            //float denom = 0.04f;
            float zoomLevel = 1000.0f;
            float amplitude = 400.0f;
            float offset = 0.1f;
            float xOffset = ((float)x) / zoomLevel;
            float yOffset = ((float)y) / zoomLevel;
            float height = noise.octaveNoise(x, y, 6);
            if (height > largestheight) largestheight = height;
            if (height < lowestHeight) lowestHeight = height;
            //height *= amplitude;
            //height -= 50.f;
            procTerrain[baseIndex] = height;
        }
    }
    Console::LogInfo("Height: %.8f, %.8f", largestheight, lowestHeight);
    // The data map has to be of size cellcout + 1 because the vertex rows are cellcount + 1 so we need another row to sample for height
    std::shared_ptr<PBRMaterial> terrainMat = std::make_shared<PBRMaterial>();
    auto terrainMesh = CreateTerrainModel(procTerrain, proceduralTerrain, 5)->CreateMesh(terrainMat);
    terrainMat->SetAlbedo(glm::vec3(0.0f, 0.8f, 0.0f));
    terrainMat->SetRoughness(1.0f);
    auto tmpTransform = new Transform();
    terrainMesh->SetTransform(tmpTransform);
    mainPipeline->AddMesh(terrainMesh);
}

float sunRotation = 40.0f;
std::shared_ptr<Light> sunLight = nullptr;
std::shared_ptr<ProceduralSkybox> skybox = nullptr;


void DrawSkyboxDebug() {
    static bool showDebug = false;
    ImGui::Checkbox("Show skybox", &showDebug);
    if(showDebug) {
        if(ImGui::Begin("Skybox", &showDebug)) {
            bool valueChanged = false;

            if (ImGui::SliderAngle("Sun rotation", &sunRotation, 0.0f)) {
                glm::vec3 rotation = glm::rotateX(glm::vec3(0, 0, -1), sunRotation);
                sunLight->GetTransform()->position = rotation;;
                valueChanged = true;
            }

            if (ImGui::Button("Refresh environment") || valueChanged) {
                mainPipeline->SetEnvironment(PBRTextureUtil::CreateEnvironment(skybox));
            }
        }
        ImGui::End();
    }
}

CameraController* cameraController = nullptr;
std::shared_ptr<Mesh> selectedMesh = nullptr;
bool hasSelectedMesh = false;

void DrawSelectedMesh() {
    if(ImGui::Begin("Selected mesh", &hasSelectedMesh)) {
        ImGui::MeshEdit(selectedMesh, true);
    }
    ImGui::End();
}

void DrawDebug() {
    DrawSkyboxDebug();
    cameraController->DrawDebug();
}

int main() {
    Console::SetPrintLevel(Console::LOG_INFO);

    int width = 1024, height = 768;
    bool enableGraphicsDebug = false;
    RenderEngine::Initialize("RenderEngine Tests", width, height, enableGraphicsDebug, true, true, true);
    mainPipeline = RenderEngine::GetMainWindow()->GetRenderPipeline();
    mainPipeline->Initialize3D(true);
    RenderEngine::InitializeImGuiAndDebugWindows();
    //RenderEngine::GetMainWindow()->SwitchToFullscrendWindowed();
    RenderEngine::SetWorldClearColor(glm::vec3(0, 0, 0));

    cameraController = new CameraController(RenderEngine::GetMainWindow());
    cameraController->SetSpeed(15.0f);
    cameraController->GetCamera()->SetZFar(100000.0f);
    ShaderUBOS::UpdateActiveCamera();

    float intensity = 10.0f;
    float lightTemp = 8000.0f;
    sunLight = std::make_shared<Light>(LightType::TYPE_DIRECTIONAL);
    sunLight->SetTemperature(lightTemp);
    sunLight->SetStrength(intensity); 
    auto sunTransform = new Transform();
    sunLight->SetTransform(sunTransform);
    glm::vec3 rotation = glm::rotateX(glm::vec3(0, 0, -1), sunRotation);
    sunTransform->position = rotation;

    mainPipeline->AddLight(sunLight);

    auto newPortTexture = std::make_shared<Texture2D>();
    newPortTexture->UploadHDRImage(ImageLoader::LoadHDRImage("AssetsNoDist/Engine/Environment/Newport_Loft/Newport_Loft_Ref.hdr"));
    auto newPortCubemap = EquirectangularTextureConverter::Convert(newPortTexture);

    auto newPortSkyboxMat = std::make_shared<CubemapMaterial>();
    newPortSkyboxMat->SetCubemap(newPortCubemap);
    auto newportSkybox = std::make_shared<Skybox>(newPortSkyboxMat);

    auto trans = new Transform();
    trans->position = glm::vec3(-500.0f);
    trans->scale = glm::vec3(1000.0f);
    skybox = std::make_shared<ProceduralSkybox>(sunLight);
    skybox->SetTransform(trans);
    newportSkybox->SetTransform(trans);

    auto quad = MeshFactory::CreateQuadMesh(std::make_shared<PBRMaterial>(), 100.0f);
    quad->SetTransform(new Transform());
    mainPipeline->AddMesh(quad);

    auto bunnyModels = ModelLoader::LoadModels("AssetsNoDist/Engine/Models/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");
    Console::LogInfo("Bunny models returned %i models", bunnyModels.size());
    for (auto bunnyModel : bunnyModels) {
        auto mat = std::make_shared<PBRMaterial>();
        auto albedo = std::make_shared<Texture2D>();
        auto normal = std::make_shared<Texture2D>();
        auto metallic = std::make_shared<Texture2D>();
        auto roughness = std::make_shared<Texture2D>();
        albedo->UploadImage(ImageLoader::LoadImage("AssetsNoDist/Engine/Models/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga"), false, true);
        normal->UploadImage(ImageLoader::LoadImage("AssetsNoDist/Engine/Models/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga"), false, true);
        metallic->UploadImage(ImageLoader::LoadImage("AssetsNoDist/Engine/Models/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga"), false, true);
        roughness->UploadImage(ImageLoader::LoadImage("AssetsNoDist/Engine/Models/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga"), false, true);
        mat->SetAlbedoTexture(albedo);
        mat->SetNormalTexture(normal);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(roughness);
        auto mesh = bunnyModel->CreateMesh(mat);
        auto trans = new Transform();
        trans->scale = glm::vec3(0.05f);
        trans->position = glm::vec3(-20, 20, 0);
        trans->rotation.SetEulerAngles(-90, 0, 0);
        mesh->SetTransform(trans);
        mainPipeline->AddMesh(mesh);
    }
    bunnyModels.clear();


    // Configure the reflection cubemap and irradiance cubemap
    //mainPipeline->SetEnvironment(PBRTextureUtil::CreateEnvironment(newportSkybox));
    mainPipeline->SetEnvironment(PBRTextureUtil::CreateEnvironment(skybox));
    mainPipeline->GetPostProcessor()->UseLinearColorSpace();
    mainPipeline->AddMesh(skybox);
    //mainPipeline->AddMesh(newportSkybox);
    Engine::Debug::CreateDebugWindow("RenderEngine test", std::bind(&DrawDebug));

    CreateSingleSphereTestScene();
    CreateMaterialTestScene();
    //CreateTerrainTestScene();

    while(!RenderEngine::GetMainWindow()->ShouldWindowClose()) {
        RenderEngine::NewFrame(0.016f);
        cameraController->Update(0.016f);

        auto io = ImGui::GetIO();

        if (!io.WantCaptureMouse && io.MouseClicked[0] && Engine::Debug::IsDebugVisible()) {
            ImVec2 pos = io.MouseClickedPos[0];
            selectedMesh = RenderEngine::GetMainWindow()->GetRenderPipeline()->MousePick(pos.x, pos.y);
            hasSelectedMesh = selectedMesh != nullptr;
        }

        if (hasSelectedMesh && Engine::Debug::IsDebugVisible()) {
            DrawSelectedMesh();
        }

        RenderEngine::Render();
        RenderEngine::EndFrame();
    }
    return 0;
}