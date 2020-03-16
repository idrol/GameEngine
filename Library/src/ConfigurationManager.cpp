//
// Created by Idrol on 12-Dec-18.
//
#include <Engine/Library/ConfigurationManager.h>
#include <Engine/Library/Console.h>
#include <stdexcept>

using namespace Engine::Library;

std::unordered_map<std::string, std::vector<std::function<void(std::any)>>> ConfigurationManager::valueChangedCallbacks;
std::unordered_map<std::string, std::any> ConfigurationManager::values;

void ConfigurationManager::RegisterValue(std::string valueName, std::any defaultValue) {
    values.insert_or_assign(valueName, defaultValue);
    valueChangedCallbacks.insert_or_assign(valueName, std::vector<std::function<void(std::any)>>());
}

std::any ConfigurationManager::ReadValue(std::string valueName) {
    auto it = values.find(valueName);
    if (it == values.end()) {
        throw std::runtime_error("Trying to read non existant value");
    }
    return it->second;
}

void ConfigurationManager::SetValue(std::string valueName, std::any value) {
    values.insert_or_assign(valueName, value);
    ValueChanged(valueName, value);
}

void ConfigurationManager::WatchValue(std::string valueName, std::function<void(std::any)> callback) {
    auto it = valueChangedCallbacks.find(valueName);
    if (it == valueChangedCallbacks.end()) {
        throw std::runtime_error("Trying to watch non existant value");
    }
    auto &vector = it->second;
    vector.push_back(callback);
}

void ConfigurationManager::ValueChanged(std::string valueName, std::any newValue) {
    auto it = valueChangedCallbacks.find(valueName);
    if (it == valueChangedCallbacks.end()) {
        throw std::runtime_error("Trying to watch non existant value");
    }
    auto &vector = it->second;
    for(auto const&callback: vector) {
        callback(newValue);
    }
}