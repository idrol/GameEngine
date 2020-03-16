#include "MousePickFramebuffer.h"

using namespace Engine::RenderEngine;

MousePickFramebuffer::MousePickFramebuffer(int width, int height): Framebuffer(width, height) {
	colorAttachment = std::make_shared<Texture2D>();
	colorAttachment->UseBilinearFiltering();
}

void MousePickFramebuffer::CreateAttachments() {
	// This is limited to a single byte id at the moment
	colorAttachment->SetFormat(width, height, GL_R32I, GL_RED_INTEGER, GL_INT);
	BindTextureAttachment(colorAttachment, GL_COLOR_ATTACHMENT0);
	depthBuffer = CreateRenderBufferAttachment(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
}

void MousePickFramebuffer::DeleteAttachments() {
	UnbindTextureAttachment(GL_COLOR_ATTACHMENT0);
	glDeleteRenderbuffers(1, &depthBuffer);
}

std::shared_ptr<Texture2D> MousePickFramebuffer::GetColorAttachment() {
	return colorAttachment;
}