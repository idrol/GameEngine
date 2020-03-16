//
// Created by idrol on 20/01/2020.
//

#ifndef ENGINE_RENDERENGINEAPI_H
#define ENGINE_RENDERENGINEAPI_H

// Defines DLL_EXPORT to be import if an outside source includes this file and defines it to be export when building the library
#ifdef BUILD_RENDERENGINE_DLL
#define RENDERENGINE_API __declspec(dllexport)
#else
#define RENDERENGINE_API __declspec(dllimport)
#endif

#endif //ENGINE_RENDERENGINEAPI_H
