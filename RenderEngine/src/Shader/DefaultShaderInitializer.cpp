//
// Created by Idrol on 20-Dec-18.
//
#include <Engine/RenderEngine/RenderEngine.h>
#include <Engine/RenderEngine/Shader/Shader.h>

#include <Engine/RenderEngine/Camera.h>
#include "Shader/PBRShader.h"
#include "Shader/LightingPassShader.h"
#include "Shader/TexturedShader.h"
#include "Shader/EquirectangularToCubemapShader.h"
#include "Shader/CubemapShader.h"
#include "Shader/PrefilterShader.h"
#include "Shader/BRDFShader.h"
#include "Shader/SSAOShader.h"
#include "Shader/ProceduralSkyboxShader.h"
#include "Shader/DefaultShaderInitializer.h"
#include "Shader/MousePickShader.h"
#include <Engine/RenderEngine/Shader/ShaderCache.h>

using namespace Engine::RenderEngine;

void DefaultShaderInitializer::CreateShaders() {
    auto pbrShader = std::make_shared<PBRShader>();
    pbrShader->Create();
    ShaderCache::RegisterShader("pbrShader", pbrShader);
    

    auto lightingPassShader = std::make_shared<LightingPassShader>();
    lightingPassShader->Create();
    ShaderCache::RegisterShader("lightingPassShader", lightingPassShader);

    auto ssaoShader = std::make_shared<SSAOShader>();
    ssaoShader->Create();
    ShaderCache::RegisterShader("ssaoShader", ssaoShader);

    auto texturedShader = std::make_shared<TexturedShader>();
    texturedShader->Create();
    ShaderCache::RegisterShader("texturedShader", texturedShader);

    auto equirectangularToCubemapShader = std::make_shared<EquirectangularToCubemapShader>();
    equirectangularToCubemapShader->Create();
    ShaderCache::RegisterShader("equirectangularToCubemapShader", equirectangularToCubemapShader);

    auto cubemap = std::make_shared<CubemapShader>();
    cubemap->Create();
    ShaderCache::RegisterShader("cubemapShader", cubemap);

    auto irradiance = std::make_shared<CubemapShader>("Assets/Engine/Shaders/cubemap.vert", "Assets/Engine/Shaders/PBR/irradiance.frag");
    irradiance->Create();
    ShaderCache::RegisterShader("irradianceShader", irradiance);

    auto prefilter = std::make_shared<PrefilterShader>();
    prefilter->Create();
    ShaderCache::RegisterShader("prefilterShader", prefilter);

    auto brdf = std::make_shared<BRDFShader>();
    brdf->Create();
    ShaderCache::RegisterShader("brdfShader", brdf);

    auto procSkybox = std::make_shared<ProceduralSkyboxShader>();
    procSkybox->Create();
    ShaderCache::RegisterShader("procSkybox", procSkybox);

    auto mousePickShader = std::make_shared<MousePickShader>();
    mousePickShader->Create();
    ShaderCache::RegisterShader("mousePick", mousePickShader);
}