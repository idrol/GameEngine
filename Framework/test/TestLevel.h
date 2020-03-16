#pragma once

#include <Engine/Framework/Level.h>

class TestLevel : public Engine::Framework::Level {
public:
	TestLevel();

	void Load() override;
	void Update(float delta) override;
	void Begin() override;
	void End() override;
	void Dispose() override;
};