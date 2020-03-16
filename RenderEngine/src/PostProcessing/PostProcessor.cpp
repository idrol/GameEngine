//
// Created by idrol on 22/01/2020.
//
#include <Engine/RenderEngine/PostProcessing/PostProcessor.h>
#include <Engine/RenderEngine/PostProcessing/PostProcess.h>
#include "Framebuffer/Graphicsbuffer.h"
#include <Engine/RenderEngine/imgui.h>

using namespace Engine::RenderEngine;

PostProcessor::PostProcessor(int width, int height) {
    this->width = width;
    this->height = height;
}

void PostProcessor::SetupPipeline(bool includeGammaCorrection) {
    gammaCorrectEnabled = includeGammaCorrection;
    toneMap = std::make_shared<PostProcess>(width, height, "Assets/Engine/Shaders/PostProcess/ExposureToneMap.frag");
    gammaCorrect = std::make_shared<PostProcess>(width, height, "Assets/Engine/Shaders/PostProcess/ApplyGamma.frag");
    AddPostProcess(toneMap);
    if(includeGammaCorrection) {
        AddPostProcess(gammaCorrect);
    }
}

void PostProcessor::UseGammaColorSpace() {
    if(gammaCorrectEnabled) return;
    postProcesses.InsertAfter(toneMap, gammaCorrect);
    gammaCorrectEnabled = true;
}

void PostProcessor::UseLinearColorSpace() {
    if(!gammaCorrectEnabled) return;
    postProcesses.Remove(gammaCorrect);
    gammaCorrectEnabled = false;
}

bool PostProcessor::IsGammaCorrectionEnabled() {
    return gammaCorrectEnabled;
}

void PostProcessor::DrawSettings() {
    static bool showPostprocessingSettings = false;
    ImGui::Checkbox("Show PostProcessor", &showPostprocessingSettings);
    if (showPostprocessingSettings) {
        ImGui::PushID("PostProcessor");
        if(ImGui::Begin("PostProcessor", &showPostprocessingSettings)) {
            bool temp = gammaCorrectEnabled;
            if (ImGui::Checkbox("Gamma Color Space", &temp)) {
                if (temp) {
                    UseGammaColorSpace();
                }
                else {
                    UseLinearColorSpace();
                }
            }
        }
        ImGui::End();
        ImGui::PopID();
    }
    
}

void PostProcessor::Resize(int width, int height) {
    this->width = width;
    this->height = height;
    while(auto node = postProcesses.Next()) {
        std::shared_ptr<PostProcess> postProcess = node->data;
        postProcess->Resize(width, height);
    }
    postProcesses.ResetCursor();
}

void PostProcessor::AddPostProcess(std::shared_ptr<PostProcess> postProcess) {
    postProcesses.Insert(std::move(postProcess));
}

std::shared_ptr<Texture2D> PostProcessor::RunPostProcessor(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> colorTexture) {
    auto currentColorTexture = colorTexture;
    while(auto node = postProcesses.Next()) {
        auto postProcess = node->data;
        auto postProcessedTexture = postProcess->Run(gBuffer, currentColorTexture);
        currentColorTexture = postProcessedTexture;
    }
    postProcesses.ResetCursor();
    return currentColorTexture;
}