#ifndef ENGINE_FRAMEWORKAPI_H
#define ENGINE_FRAMEWORKAPI_H

// Defines DLL_EXPORT to be import if an outside source includes this file and defines it to be export when building the FRAMEWORK
#ifdef BUILD_FRAMEWORK_DLL
#define FRAMEWORK_API __declspec(dllexport)
#else
#define FRAMEWORK_API __declspec(dllimport)
#endif

#endif //ENGINE_FRAMEWORKAPI_H
