//
// Created by Idrol on 20-Feb-19.
//
#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Atlas/Atlas.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>

namespace Engine::RenderEngine {
    class RENDERENGINE_API PBRAtlas: public Atlas {
    public:
        PBRAtlas(int width, int height, int channels = 3);
        // Usefull for combining with PathBuilder
        bool AddAlbedoTexture(std::string name, int padding ,std::string albedoPath);
        bool AddTexture(std::string name, int padding, std::vector<std::string> pbrPaths);
        bool AddTexture(std::string name, int padding, std::string albedoPath, std::string normalPath,
                std::string roughnessPath, std::string metallicPath);
        bool AddTexture(std::string name, int padding, std::shared_ptr<Image> albedoImage,
                std::shared_ptr<Image> normalImage,
                std::shared_ptr<Image> roughnessImage,
                std::shared_ptr<Image> metallicImage);
        std::shared_ptr<Image> GetAlbedoImage();
        std::shared_ptr<Image> GetNormalImage();
        std::shared_ptr<Image> GetRoughnessImage();
        std::shared_ptr<Image> GetMetallicImage();
        std::shared_ptr<Texture2D> GetAlbedoTexture() {return albedoTexture;};
        std::shared_ptr<Texture2D> GetNormalTexture() {return normalTexture;};
        std::shared_ptr<Texture2D> GetRoughnessTexture() {return roughnessTexture;};
        std::shared_ptr<Texture2D> GetMetallicTexture() {return metallicTexture;};

        void RegisterErrorTexture(std::vector<std::string> pbrPaths);
        glm::vec4 GetErrorUV() override;

        void EnableLinearFiltering() { useLinearFiltering = true;};

        void RenderAtlasDebug() override;

    protected:
        void UpdateTexture() override;

    private:
        std::shared_ptr<Image> albedoImage;
        std::shared_ptr<Image> normalImage;
        std::shared_ptr<Image> roughnessImage;
        std::shared_ptr<Image> metallicImage;

        std::shared_ptr<Texture2D> albedoTexture, normalTexture, roughnessTexture, metallicTexture;
        bool hasAtlasChanged;
        bool useLinearFiltering = false;
        glm::vec4 errorUV = glm::vec4(0,0,1,1);
    };
}