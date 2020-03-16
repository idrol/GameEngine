#ifndef ENGINE_NETWORKAPI_H
#define ENGINE_NETWORKAPI_H

// Defines DLL_EXPORT to be import if an outside source includes this file and defines it to be export when building the library
#ifdef BUILD_NETWORK_DLL
#define NETWORK_API __declspec(dllexport)
#else
#define NETWORK_API __declspec(dllimport)
#endif

#endif
