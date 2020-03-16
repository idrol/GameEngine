#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>

namespace Engine::RenderEngine {
	class RENDERENGINE_API HDRImage {
	public:
		HDRImage(float* date, int width, int height, int channels);
		~HDRImage();

		float* GetData();

		int GetWidth();
		int GetHeight();
		int GetChannels();
		bool IsValid();

	private:
		int width, height, channels;
		float* data;
	};
}