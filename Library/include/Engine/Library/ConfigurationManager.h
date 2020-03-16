//
// Created by Idrol on 12-Dec-18.
//
#pragma once

#include <Engine/Library/LibraryAPI.h>
#include <vector>
#include <map>
#include <functional>
#include <any>
//#include <boost/function.hpp>
//#include <GLM/glm.hpp>

namespace Engine::Library {
    class ConfigurationManager {
    public:
        LIBRARY_API static void RegisterValue(std::string valueName, std::any defaultValue);
        LIBRARY_API static std::any ReadValue(std::string valueName);
        LIBRARY_API static void SetValue(std::string valueName, std::any value);
        LIBRARY_API static void WatchValue(std::string valueName, std::function<void(std::any)> callback);
    private:
        static void ValueChanged(std::string valueName, std::any newValue);

        // Callback functions stored by string keys where a key can be for example window for value changes for window settings.
        static std::unordered_map<std::string, std::vector<std::function<void(std::any)>>> valueChangedCallbacks;
        static std::unordered_map<std::string, std::any> values;
    };
}