#pragma once

#include <memory>

namespace Engine::Framework {
	class Entity;
}

namespace ImGui {
	// Display entity edit widget
	// returns true if entity kill was requested
	bool EntityEdit(std::shared_ptr<Engine::Framework::Entity> entity);
}