#ifndef ENGINE_PHYSICSAPI_H
#define ENGINE_PHYSICSAPI_H

// Defines DLL_EXPORT to be import if an outside source includes this file and defines it to be export when building the library
#ifdef BUILD_PHYSICS_DLL
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

#endif //ENGINE_PHYSICSAPI_H
