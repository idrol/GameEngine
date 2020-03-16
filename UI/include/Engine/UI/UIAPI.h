#ifndef ENGINE_UIAPI_H
#define ENGINE_UIAPI_H

#ifdef BUILD_UI_DLL
#define UI_API __declspec(dllexport)
#else
#define UI_API __declspec(dllimport)
#endif

#endif // !ENGINE_UIAPI_H
