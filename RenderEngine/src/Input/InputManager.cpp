#include <Engine/Input/InputManager.h>
#include <Engine/Input/InputKeys.h>
#include <Engine/RenderEngine/Window.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Engine/Library/Console.h>
#include <iostream>

using namespace Engine::Input;
using namespace Engine::Library;
using namespace Engine::RenderEngine;

void keyCallback(GLFWwindow *ptr, int key, int scanCode, int action, int mods) {
	std::weak_ptr<InputManager> inputManagerInstance = ((Window*)glfwGetWindowUserPointer(ptr))->GetInputManager();
	if (inputManagerInstance.expired()) {
		Console::LogError("[InputManager] GLFW invoked input callback but the registered window does not have a valid input manager associated. Event will be ignored");
		return;
	}

	std::shared_ptr<InputManager> mngrInst = inputManagerInstance.lock();
	for (auto& callback : mngrInst->keyCallbacks) {
		callback(key, scanCode, action, mods);
	}

	if (key == KEY_UNKNOWN) return; // Only glfw known key supported for key states

	mngrInst->SetKeyState(key, action);
}

void charCallback(GLFWwindow* ptr, unsigned int charCode) {
	std::weak_ptr<InputManager> inputManagerInstance = ((Window*)glfwGetWindowUserPointer(ptr))->GetInputManager();
	if (inputManagerInstance.expired()) {
		Console::LogError("[InputManager] GLFW invoked input callback but the registered window does not have a valid input manager associated. Event will be ignored");
		return;
	}
	
	std::shared_ptr<InputManager> mngrInst = inputManagerInstance.lock();
	for (auto& callback : mngrInst->charCallbacks) {
		callback(charCode);
	}
}

void mouseButtonCallback(GLFWwindow* ptr, int mouseButton, int scancode, int action) {
	std::weak_ptr<InputManager> inputManagerInstance = ((Window*)glfwGetWindowUserPointer(ptr))->GetInputManager();
	if (inputManagerInstance.expired()) {
		Console::LogError("[InputManager] GLFW invoked input callback but the registered window does not have a valid input manager associated. Event will be ignored");
		return;
	}
	std::shared_ptr<InputManager> mngrInst = inputManagerInstance.lock();
	for (auto& callback : mngrInst->mouseButtonCallbacks) {
		callback(mouseButton, scancode, action);
	}

	if (mouseButton == KEY_UNKNOWN) return;

	mngrInst->SetMouseButtonState(mouseButton, action);
}

void cursorPositionCallback(GLFWwindow* ptr, double xPos, double yPos) {
	std::weak_ptr<InputManager> inputManagerInstance = ((Window*)glfwGetWindowUserPointer(ptr))->GetInputManager();
	if (inputManagerInstance.expired()) {
		Console::LogError("[InputManager] GLFW invoked input callback but the registered window does not have a valid input manager associated. Event will be ignored");
		return;
	}
	std::shared_ptr<InputManager> mngrInst = inputManagerInstance.lock();
	for (auto& callback : mngrInst->cursorPositionCallbacks) {
		callback(xPos, yPos);
	}
	
	auto currentPosition = mngrInst->GetMousePosition();
	double xDelta = xPos - currentPosition.x;
	double yDelta = yPos - currentPosition.y;
	auto lastDelta = mngrInst->GetMouseDelta();
	mngrInst->SetMouseDelta(glm::dvec2(lastDelta.x + xDelta, lastDelta.y + yDelta));
	mngrInst->SetMousePosition(glm::dvec2(xPos, yPos));
}

void scrollCallback(GLFWwindow* ptr, double xOffset, double yOffset) {
	std::weak_ptr<InputManager> inputManagerInstance = ((Window*)glfwGetWindowUserPointer(ptr))->GetInputManager();
	if (inputManagerInstance.expired()) {
		Console::LogError("[InputManager] GLFW invoked input callback but the registered window does not have a valid input manager associated. Event will be ignored");
		return;
	}
	std::shared_ptr<InputManager> mngrInst = inputManagerInstance.lock();
	for (auto& callback : mngrInst->scrollCallbacks) {
		callback(xOffset, yOffset);
	}

	mngrInst->SetScrollOffset(glm::dvec2(xOffset, xOffset));
}

InputManager::InputManager(std::shared_ptr<Window> window) {
	this->window = window;
	glfwSetKeyCallback(window->window, keyCallback);
	glfwSetCharCallback(window->window, charCallback);
	glfwSetMouseButtonCallback(window->window, mouseButtonCallback);
	glfwSetCursorPosCallback(window->window, cursorPositionCallback);
	glfwSetScrollCallback(window->window, scrollCallback);
}

InputState& const InputManager::GetInputState() {
	return state;
}

void InputManager::EndFrame() {
	SetMouseDelta(glm::dvec2(0, 0));
	SetScrollOffset(glm::dvec2(0, 0));

	// GLFW only sends one PRESS action and waits for a bit until it starts sending repeat actions but the input manager should only have the press event the frame it happens
	for (auto& it : state.keyStates) {
		if (it.second == ACTION_PRESS) {
			it.second = ACTION_REPEAT;
		}
	}
	for (auto& it : state.mouseButtonState) {
		if (it.second == ACTION_PRESS) {
			it.second = ACTION_REPEAT;
		}
	}
}

int InputManager::GetKeyState(int key) {
	auto it = state.keyStates.find(key);
	if (it == state.keyStates.end()) {
		return ACTION_RELEASE;
	}
	return it->second;
}

bool InputManager::IsKeyHeld(int key) {
	return GetKeyState(key) != ACTION_RELEASE;
}

bool InputManager::IsKeyPressed(int key) {
	return GetKeyState(key) == ACTION_PRESS;
}

int InputManager::GetMouseButtonState(int button) {
	auto it = state.mouseButtonState.find(button);
	if (it == state.mouseButtonState.end()) {
		return ACTION_RELEASE;
	}
	return it->second;
}

bool InputManager::IsMouseButtonHeld(int button) {
	return GetMouseButtonState(button) != ACTION_RELEASE;
}

bool InputManager::IsMouseButtonPressed(int button) {
	return GetMouseButtonState(button) == ACTION_PRESS;
}

glm::dvec2& const InputManager::GetMousePosition() {
	return state.mousePosition;
}

glm::dvec2& const InputManager::GetMouseDelta() {
	return state.mouseDelta;
}

glm::dvec2& const InputManager::GetScrollOffset() {
	return state.scrollOffset;
}

void InputManager::ShowMouse() {
	if (window.expired()) return;
	window.lock()->ShowCursor();
	state.mouseHidden = false;
}

void InputManager::HideMouse() {
	if (window.expired()) return;
	window.lock()->HideCursor();
	state.mouseHidden = true;
}

bool& const InputManager::IsMouseHidden() {
	return state.mouseHidden;
}

void InputManager::RegisterKeyCallback(std::function<void(int, int, int, int)> callback) {
	keyCallbacks.push_back(callback);
}

void InputManager::RegisterCharCallback(std::function<void(unsigned int)> callback) {
	charCallbacks.push_back(callback);
}

void InputManager::RegisterMouseButtonCallback(std::function<void(int, int, int)> callback) {
	mouseButtonCallbacks.push_back(callback);
}

void InputManager::RegisterMousePositionCallback(std::function<void(double, double)> callback) {
	cursorPositionCallbacks.push_back(callback);
}

void InputManager::RegisterScrollCallback(std::function<void(double, double)> callback) {
	scrollCallbacks.push_back(callback);
}

void InputManager::SetKeyState(int key, int state) {
	this->state.keyStates[key] = state;
}

void InputManager::SetMouseButtonState(int key, int state) {
	this->state.mouseButtonState[key] = state;
}

void InputManager::SetMousePosition(glm::dvec2 position) {
	state.mousePosition = position;
}

void InputManager::SetMouseDelta(glm::dvec2 delta) {
	state.mouseDelta = delta;
}

void InputManager::SetScrollOffset(glm::dvec2 offset) {
	state.scrollOffset = offset;
}