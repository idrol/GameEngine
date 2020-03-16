#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Atlas/Atlas.h>
#include <Engine/RenderEngine/Texture/Texture2D.h>
//#include "Engine/PathBuilder.h"


namespace Engine::RenderEngine {
class Image;

class RENDERENGINE_API AtlasTexture: public Atlas {
public:
    AtlasTexture(int width, int height, int channels = 3);
    //bool AddTexture(TexturePath path, int padding = 0);
    bool AddTexture(std::string name, int padding, std::string path);
    bool AddTexture(std::string name, int padding, std::shared_ptr<Image> imageToAdd);
    std::shared_ptr<Image> GetImage();
    std::shared_ptr<Texture2D> GetTexture() {return texture;};
    void RenderAtlasDebug() override;

protected:
    void UpdateTexture() override;

    std::shared_ptr<Texture2D> texture;
    bool hasAtlasChanged = false;
    std::shared_ptr<Image> image;
};
}