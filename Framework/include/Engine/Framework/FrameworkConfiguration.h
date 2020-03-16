#pragma once

#include <Engine/Framework/FrameworkAPI.h>
#include <string>

namespace Engine::Framework {
	// Use for initializing the engine framework
	// This cannot be changed att runtime
	struct FRAMEWORK_API FrameworkConfiguration{
		// Render engine init
		std::string mainWindowTitle = "Engine framework";
		int mainWindowWidth = 1024;
		int mainWindowHeight = 768;
		bool mainWindowAutoFocus = false;
		bool mainWindowMaximize = false;
		// This overrides auto focus and maximize configs
		bool mainWindowStartInFullscreen = false;
		// If true OpenGL core specification is used. If false compat is used with GL extensions
		bool useCoreGL = false;

		// Render engine setup
		// Enable graphics debuger (prints out more detailed error messages on api missuse or performance optimization hints or int some cases just spam)
		// (This information is logged by the underlying graphics driver so blame NVIDIA or AMD)
		bool enableGraphicsDebug = false;
		// Enable imgui rendering
		bool enableImGui = false;
		
		// Enable 3D renderer. (requires you to register a camera with the renderpipeline or else the render engine spams errors and 3D rendering is effectivly disabled)
		bool enable3D = false;
		// If 3D is enabled mouse picking can be enabled which makes the RenderPipeline::MousePick(x, y) available which returns the mesh rendered at x and y
		bool enable3DMousePicking = false;

		// Should gamma correction be disabled on initialization ?
		// This can be changed att runtime
		bool useLinearColorSpace = false;
		
		// Enable 2D renderer
		bool enable2D = false;

		// Enable ui requires 2D rendering
		bool enableUI = false;
	};
}