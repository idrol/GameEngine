#include <Engine/RenderEngine/Texture/HDRImage.h>
#include <malloc.h>

using namespace Engine::RenderEngine;

HDRImage::HDRImage(float* data, int width, int height, int channels) {
	this->data = data;
	this->width = width;
	this->height = height;
	this->channels = channels;
}

HDRImage::~HDRImage() {
	free(data);
}

float* HDRImage::GetData() {
	return data;
}

int HDRImage::GetWidth() {
	return width;
}

int HDRImage::GetHeight() {
	return height;
}

int HDRImage::GetChannels() {
	return channels;
}

bool HDRImage::IsValid() {
	return data != nullptr;
}