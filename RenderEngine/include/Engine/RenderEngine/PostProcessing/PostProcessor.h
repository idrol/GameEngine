//
// Created by idrol on 22/01/2020.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/Library/Container/LinkedList.hpp>
#include <Engine/RenderEngine/Texture/Texture2D.h>


namespace Engine::RenderEngine {
    class PostProcess;
    class Graphicsbuffer;
    class RENDERENGINE_API PostProcessor {
    public:
        PostProcessor(int width, int height);

        void DrawSettings();

        void UseGammaColorSpace();
        void UseLinearColorSpace();
        bool IsGammaCorrectionEnabled();
        void Resize(int width, int height);
        void SetupPipeline(bool includeGammaCorrection = true);
        void AddPostProcess(std::shared_ptr<PostProcess> postProcess);
        // Run the post process pipeline. The post processor exposes the graphics buffer as extra information to the post processing pipeline
        std::shared_ptr<Texture2D> RunPostProcessor(std::shared_ptr<Graphicsbuffer> gBuffer, std::shared_ptr<Texture2D> colorTexture);
    private:
        bool gammaCorrectEnabled = true;
        std::shared_ptr<PostProcess> toneMap;
        std::shared_ptr<PostProcess> gammaCorrect;
        LinkedList<std::shared_ptr<PostProcess>> postProcesses;
        int width, height;
    };
}