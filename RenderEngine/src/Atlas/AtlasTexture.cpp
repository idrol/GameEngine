#include <Engine/RenderEngine/Atlas/AtlasTexture.h>
#include <Engine/RenderEngine/Texture/ImageLoader.h>
#include <Engine/Library/Console.h>

//#include "Engine/Atlas/AtlasTexture.h"
//#include "Engine/Graphics/Texture/Image.h"
//#include "Engine/Graphics/Texture/TextureUtil.h"
//#include "Engine/Debug/Console.h"
//#include "Engine/ImGui/imgui.h"

using namespace Engine::RenderEngine;

AtlasTexture::AtlasTexture(int width, int height, int channels): Atlas(width, height) {
    image = std::make_shared<Image>(width, height, channels);
    texture = std::make_shared<Texture2D>();
}

/*bool AtlasTexture::AddTexture(TexturePath path, int padding) {
    return this->AddTexture(path.name, padding, path.path);
}*/

bool AtlasTexture::AddTexture(std::string name, int padding, std::string path) {
    auto image = ImageLoader::LoadImage(path);
    return this->AddTexture(name, padding, image);
}

bool AtlasTexture::AddTexture(std::string name, int padding, std::shared_ptr<Image> imageToAdd) {
    auto node = root.Insert(imageToAdd, padding);
    if (node == nullptr) return false;

    rectMap[name] = node->rectangle;
    textureCoordinates[name] = glm::vec4(
            (float)node->rectangle.x/root.rectangle.width,
            (float)node->rectangle.y/root.rectangle.height,
            (float)(node->rectangle.x+node->rectangle.width)/root.rectangle.width,
            (float)(node->rectangle.y+node->rectangle.height)/root.rectangle.height
            );
    auto rect = node->rectangle;
    image->DrawImage(imageToAdd, rect.x, rect.y, rect.width, rect.height);
    hasAtlasChanged = true;
    return true;
}

std::shared_ptr<Image> AtlasTexture::GetImage() {
    return image;
}

void AtlasTexture::UpdateTexture() {
    if(hasAtlasChanged) {
        texture->UploadImage(image);
        hasAtlasChanged = false;
    }
}

void AtlasTexture::RenderAtlasDebug() {
    /*if(textureHandle > 0) {
        ImGui::Image(reinterpret_cast<void*>(textureHandle), ImVec2(10000, 10000), ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1), ImVec4(.7f, .7f, .7f, 1));
    }*/
}