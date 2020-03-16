#pragma once

#include <string>
#include <memory>
#include <GLM/glm.hpp>
#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <string>

namespace Engine::RenderEngine {
class RENDERENGINE_API Image {
public:
    // Create an image using input data, Note: Image takes ownership of data and manages it!
    Image(unsigned char *data, int width, int height, int channels);
    Image(int width, int height, int channels);
    
    ~Image();

    //void DrawImage(std::shared_ptr<Image> imageToDraw, Rectangle drawArea);

    // Warning function does not support drawing image in different size than source
    void DrawImage(std::shared_ptr<Image> imageToDraw, int x, int y, int width, int height);

    void Fill(unsigned char r);
    void Fill(unsigned char  r, unsigned char  g);
    void Fill(unsigned char  r, unsigned char  g, unsigned char  b);
    void Fill(unsigned char  r, unsigned char  g, unsigned char  b, unsigned char  a);
    
    bool IsValid();
    
    // Get raw pointer to data, Data is managed by Image object, don't delete or free!
    unsigned char *GetData();
    
    int GetWidth();
    
    int GetHeight();
    
    // Get the number of color channels for this image, R = 1, RG = 2, RGB = 3, RGBA = 4
    int GetChannels();
    
    // Expands single red channel image into full RGBA texture (ex. RRRR if dstChannel is 4),
    // if redToAlpha is set then the red channel will be set to alpha and the rest of the channels will be white
    // (0xFF 0xFF 0xFF R)
    void ExpandRedToRGBA(int dstChannelCount, bool redToAlpha = false);

private:
    
    // Size of image
    int width, height;
    
    // Calculates index into data array based on width of image and channel count per pixel
    int GetArrayIndex(int x, int y, int width, int channels);
    
    unsigned char *data;
    
    int channels;
};
}