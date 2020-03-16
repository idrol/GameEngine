#include <iostream>
#include <GL/glew.h>
#include <Engine/RenderEngine/Texture/Image.h>
#include <Engine/Library/Console.h>

using namespace Engine::Library;
using namespace Engine::RenderEngine;

Image::Image(unsigned char *data, int width, int height, int channels) {
    this->data = data;
    this->width = width;
    this->height = height;
    this->channels = channels;
}

Image::Image(int width, int height, int channels) :
    Image((unsigned char*)malloc(static_cast<size_t>(width * height * channels)),
        width,
        height,
        channels) {

}

Image::~Image() {
    free(data);
}

void Image::Fill(unsigned char r) {
    for(int yPos = 0; yPos < height; yPos++) {
        for(int xPos = 0; xPos < width; xPos++) {
            int drawIndex = GetArrayIndex(xPos, yPos, width, channels);
            data[drawIndex] = r;
        }
    }
}

void Image::Fill(unsigned char  r, unsigned char  g) {
    if(channels<2) {
        Console::LogError("Cannot fill image with two channels when image has less than two channels.");
        return;
    }
    for(int yPos = 0; yPos < height; yPos++) {
        for(int xPos = 0; xPos < width; xPos++) {
            int drawIndex = GetArrayIndex(xPos, yPos, width, channels);
            data[drawIndex] = r;
            data[drawIndex+1] = g;
        }
    }
}

void Image::Fill(unsigned char  r, unsigned char  g, unsigned char  b) {
    if(channels < 3) {
        Console::LogError("Cannot fill image with three channels when image has less than three channels.");
    }
    for(int yPos = 0; yPos < height; yPos++) {
        for(int xPos = 0; xPos < width; xPos++) {
            int drawIndex = GetArrayIndex(xPos, yPos, width, channels);
            data[drawIndex] = r;
            data[drawIndex+1] = g;
            data[drawIndex+2] = b;
        }
    }
}

void Image::Fill(unsigned char  r, unsigned char  g, unsigned char  b, unsigned char  a) {
    if(channels < 4) {
        Console::LogError("Cannot fill image with four channels when image has less than four channels.");
    }
    for(int yPos = 0; yPos < height; yPos++) {
        for(int xPos = 0; xPos < width; xPos++) {
            int drawIndex = GetArrayIndex(xPos, yPos, width, channels);
            data[drawIndex] = r;
            data[drawIndex+1] = g;
            data[drawIndex+2] = b;
            data[drawIndex+3] = a;
        }
    }
}
/*
void Image::DrawImage(std::shared_ptr<Image> imageToDraw, Rectangle drawArea) {
    this->DrawImage(std::move(imageToDraw), drawArea.x, drawArea.y, drawArea.width, drawArea.height);
}*/

void Image::DrawImage(std::shared_ptr<Image> image, int x, int y, int width, int height) {
    if(width > this->width || height > this->height || x+width > this->width || y+height > this->height) {
        std::cout << "[Image] Tried drawing image outside boundries" << std::endl;
        exit(-1);
    }
    if(width != image->width || height != image->height) {
        std::cout << "[Image] Cannot draw image when width or height is different than source" << std::endl;
        exit(-1);
    }
    int srcX = 0;
    int srcY = 0;
    for(int yPos=y;yPos<y+height;yPos++) {
        for(int xPos=x;xPos<x+width;xPos++) {
            int drawIndex = GetArrayIndex(xPos, yPos, this->width, this->channels);
            int srcIndex = GetArrayIndex(srcX, srcY, width, image->channels);
            data[drawIndex] = image->data[srcIndex]; // R
            if(this->channels >= 2) {
                if(image->channels >= 2) {
                    data[drawIndex+1] = image->data[srcIndex+1]; // G
                } else {
                    data[drawIndex+1] = 0x00;
                }
            }
            if(this->channels >= 3) {
                if(image->channels >= 3) {
                    data[drawIndex+2] = image->data[srcIndex+2]; // B
                } else {
                    data[drawIndex+2] = 0x00;
                }
            }
            if (this->channels >= 4) {
                if(image->channels >= 4) {
                    data[drawIndex+3] = image->data[srcIndex+3]; // A
                } else {
                    data[drawIndex+3] = 0xFF;
                }
            }
            srcX++;
        }
        srcY++;
        srcX = 0;
    }
}

void Image::ExpandRedToRGBA(int dstChannelCount, bool redToAlpha) {
    if(channels == 0 || !data) return;
    unsigned char *newData = (unsigned char*)malloc(static_cast<size_t>(width*height*dstChannelCount));
    for(int yPos = 0;yPos<height;yPos++) {
        for(int xPos = 0;xPos<width;xPos++) {
            
            int drawIndex = GetArrayIndex(xPos, yPos, width, dstChannelCount);
            int srcIndex = GetArrayIndex(xPos, yPos, width, channels);
            
            if(redToAlpha)
                newData[drawIndex] = 0xFF; // R
            else
                newData[drawIndex] = data[srcIndex]; // R
            
            if(dstChannelCount >= 2) {
                if(channels >= 2) {
                    newData[drawIndex+1] = data[srcIndex+1]; // G
                } else {
                    if(redToAlpha) {
                        newData[drawIndex+1] = 0xFF;
                    } else {
                        newData[drawIndex+1] = data[srcIndex];
                    }
                }
            }
            if(dstChannelCount >= 3) {
                if(channels >= 3) {
                    newData[drawIndex+2] = data[srcIndex+2]; // B
                } else {
                    if(redToAlpha) {
                        newData[drawIndex+2] = 0xFF;
                    } else {
                        newData[drawIndex+2] = data[srcIndex];
                    }
                }
            }
            if (dstChannelCount >= 4) {
                if(channels >= 4) {
                    newData[drawIndex+3] = data[srcIndex+3]; // A
                } else {
                    if(redToAlpha) {
                        newData[drawIndex+3] = data[srcIndex];
                    } else {
                        newData[drawIndex+3] = 0xFF;
                    }
                }
            }
        }
    }
    free(data);
    data = newData;
    channels = dstChannelCount;
}

int Image::GetArrayIndex(int x, int y, int width, int channels) {
    return ((y*width)+x)*channels;
}

bool Image::IsValid() {
    return data != nullptr;
}

unsigned char *Image::GetData() {
    return data;
}

int Image::GetWidth() {
    return width;
}

int Image::GetHeight() {
    return height;
}

int Image::GetChannels() {
    return channels;
}
