#pragma once

#include <string>

namespace Engine::RenderEngine {
	// Runs a preprocess task to allow includes on all versions and implementations of opengl
	// Searches and copy pastes include paths where found so we don't have to rely on the drivers glsl compiler for includes
	// This is needed since there seems to be lacking support for includes on some drivers and vendors
	class ShaderPreprocessor {
	public:
		// Load shader file and do one preprocess pass
		static std::string LoadShader(std::string file);
		// Preprocess pass only does the first level this means that includes inside and include are not preprocessed and will cause glsl errors
		static std::string Preprocess(std::string data);
	};
}