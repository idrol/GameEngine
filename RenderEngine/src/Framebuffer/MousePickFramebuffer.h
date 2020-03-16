#pragma once

#include <Engine/RenderEngine/Framebuffer/Framebuffer.h>

namespace Engine::RenderEngine {
	class MousePickFramebuffer : public Framebuffer {
	public:
		MousePickFramebuffer(int width, int height);
		void CreateAttachments() override;
		void DeleteAttachments() override;

		std::shared_ptr<Texture2D> GetColorAttachment();

	protected:
		std::shared_ptr<Texture2D> colorAttachment = nullptr;
		unsigned int depthBuffer = 0;
	};
}