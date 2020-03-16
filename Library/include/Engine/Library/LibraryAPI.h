//
// Created by idrol on 20/01/2020.
//
#ifndef ENGINE_LIBRARYAPI_H
#define ENGINE_LIBRARYAPI_H

// Defines DLL_EXPORT to be import if an outside source includes this file and defines it to be export when building the library
#ifdef BUILD_LIBRARY_DLL
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif

#endif //ENGINE_LIBRARYAPI_H
