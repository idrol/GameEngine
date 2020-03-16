#include <Engine/RenderEngine/Texture/ImageLoader.h>
#include <Engine/Library/Console.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Engine::RenderEngine;
using namespace Engine::Library;

std::shared_ptr<Image> ImageLoader::LoadImage(std::string filePath) {
    int width, height, channels;
    unsigned char* data;
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        Console::LogError("Could not load image %s", filePath.c_str());
        width = height = channels = 0;
    }
    return std::make_shared<Image>(data, width, height, channels);
}

std::shared_ptr<HDRImage> ImageLoader::LoadHDRImage(std::string filePath) {
    int width, height, channels;
    float* data;
    stbi_set_flip_vertically_on_load(true);
    data = stbi_loadf(filePath.c_str(), &width, &height, &channels, 0);
    if (!data)
    {
        Console::LogError("Could not load image %s", filePath.c_str());
        width = height = channels = 0;
    }
    return std::make_shared<HDRImage>(data, width, height, channels);
}