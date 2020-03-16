#pragma once

#include <Engine/RenderEngine/RenderEngineAPI.h>
#include <Engine/RenderEngine/Window.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <GLM/glm.hpp>

namespace Engine::Input {
	// The internal inputState for the input manager
	struct InputState {
		std::unordered_map<int, int> keyStates;
		std::unordered_map<int, int> mouseButtonState;
		glm::dvec2 mousePosition; // The current mouse position. The position is undefined while mouseHidden = true as glfw hides and "grabs" the mouse
		glm::dvec2 mouseDelta; // The mouse position change since last frame
		glm::dvec2 scrollOffset;
		double mouseX, mouseY; 
		double deltaMouseX, deltaMouseY; 
		double scrollXOffset, scrollYOffset;
		bool mouseHidden = false; // Is the mouse hidden
	};

	class InputManager {
	public:
		RENDERENGINE_API InputManager(std::shared_ptr<Engine::RenderEngine::Window> window);

		RENDERENGINE_API InputState& const GetInputState();

		// Returns true when the key state is PRESSED and REPEAT
		RENDERENGINE_API bool IsKeyHeld(int keyCode);
		// Returns true only when the key state is PRESSED. NOTE the state will only be PRESSED for a small amount of time and it is possible to miss this state ass 
		RENDERENGINE_API bool IsKeyPressed(int keyCode);
		RENDERENGINE_API int GetKeyState(int keyCode);

		RENDERENGINE_API bool IsMouseButtonHeld(int mouseButton);
		RENDERENGINE_API bool IsMouseButtonPressed(int mouseButton);
		RENDERENGINE_API int GetMouseButtonState(int mouseButton);

		RENDERENGINE_API glm::dvec2& const GetMousePosition();
		RENDERENGINE_API glm::dvec2& const GetMouseDelta();
		RENDERENGINE_API glm::dvec2& const GetScrollOffset();

		RENDERENGINE_API void ShowMouse();
		RENDERENGINE_API void HideMouse();

		RENDERENGINE_API bool& const IsMouseHidden();

		RENDERENGINE_API void RegisterKeyCallback(std::function<void(int, int, int, int)> callback);
		RENDERENGINE_API void RegisterCharCallback(std::function<void(unsigned int)> callback);
		RENDERENGINE_API void RegisterMouseButtonCallback(std::function<void(int, int, int)> callback);
		RENDERENGINE_API void RegisterMousePositionCallback(std::function<void(double, double)> callback);
		RENDERENGINE_API void RegisterScrollCallback(std::function<void(double, double)> callback);

		//Internal functions that changes the state
		void EndFrame();

		void SetKeyState(int key, int state);
		void SetMouseButtonState(int key, int state);
		void SetMousePosition(glm::dvec2 position);
		void SetMouseDelta(glm::dvec2 delta);
		void SetScrollOffset(glm::dvec2 scrollOffset);

		// Callbacks
		std::vector <std::function<void(int, int, int, int)>> keyCallbacks;
		std::vector < std::function<void(unsigned int)>> charCallbacks;
		std::vector < std::function<void(int, int, int)>> mouseButtonCallbacks;
		std::vector < std::function<void(double, double)>> cursorPositionCallbacks;
		std::vector < std::function<void(double, double)>> scrollCallbacks;

	private:
		std::weak_ptr<Engine::RenderEngine::Window> window;
		
		// State
		InputState state;
	};
}