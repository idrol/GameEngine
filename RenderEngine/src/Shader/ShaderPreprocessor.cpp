#include "ShaderPreprocessor.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <Engine/Library/Console.h>
#include <regex>

using namespace Engine::RenderEngine;
using namespace Engine::Library;


std::vector<std::string> split(const std::string& s, char delimeter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimeter)) {
		tokens.push_back(token);
	}
	return tokens;
}

std::string extractIncludePath(std::string line) {
    std::regex pathExtract(R"(".*?")");
    std::smatch paths;
    std::regex_search(line, paths, pathExtract);
    std::string path = paths[0];
    path = path.substr(1, path.length() - 2);
    return path;
}

std::string ShaderPreprocessor::LoadShader(std::string file) {
    //Console::LogInfo("Loading shader %s", file.c_str());
    std::string data;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        //Console::LogInfo("[Shader] Reading file %s", file.c_str());
        shaderFile.open(file);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        data = shaderStream.str();
    }
    catch (std::ifstream::failure e) {
        Console::LogError("Could not read shader %s", file.c_str());
        throw std::runtime_error("Shader not found see error log");
    }
    //Console::LogInfo("Shader data: %s", data.c_str());
    return Preprocess(data);
}

std::string ShaderPreprocessor::Preprocess(std::string data) {
	std::vector<std::string> splitData = split(data, '\n');
    // The first line contains #version
    // n numbers of lines after should contain #include
    // when a line doesnt have #include then no more #include are expected
    std::regex includeMatch("#include.*");

    for (int i = 0; i < splitData.size(); i++) {
        if (std::regex_match(splitData[i], includeMatch)) {
            // This line contains an include
            std::string path = extractIncludePath(splitData[i]);
            splitData[i] = LoadShader(path);
        }

        /*if (std::regex_match(splitData[i], includeMatch)) {
            Console::LogInfo("Found include match");
            std::smatch m;
            std::regex_search(splitData[i], m, pathExtract);
            for (const std::string& match : m) {
                Console::LogInfo("Match %s", match.c_str());
            }
            std::string extractedPath = m[0];
            //Console::LogInfo("Matched path %s", extractedPath.c_str());
        }*/
    }
    std::stringstream finalData;
    for (int i = 0; i < splitData.size(); i++) {
        finalData << splitData[i];
        if (i != splitData.size() - 1) {
            finalData << '\n';
        }
    }
    return finalData.str();
}