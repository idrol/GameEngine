#pragma once
//
// Created by Patrick on 09/11/2018.
//

#include <vector>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <cstdlib>
#include <string>
#include <functional>
#include <Engine/Library/LibraryAPI.h>

namespace Engine::Library {
    // Portable helpers
    static inline int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
    static inline int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
    static inline char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
    static inline void  Strtrim(char* str)                                  { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }


     class LIBRARY_API Console {
    public:


         static void Log(const char* format, ...);
         static void LogInfo(const char* format, ...);
         static void LogError(const char* format, ...);
         static void LogWarn(const char* format, ...);
         static void FatalError(const char* format, ...);

         static void ClearLogs();


         static void SetPrintLevel(int printLevel);

         static const std::vector<char*> GetHistory();

         static const int LOG_DISABLED = 0;
         static const int LOG_ERROR = 1;
         static const int LOG_WARN = 2;
         static const int LOG_INFO = 3;
         static const char *LOG_FILE;


    private:
        // Specifiecs what kind of log level is printed out. 0 = disabled, 1 = Error, 2 = Warn, 3 = Info
        static int printLevel;
    };
}