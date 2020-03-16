//
// Created by Patrick on 09/11/2018.
//
#include <stdarg.h>
#include <sstream>
#include <map>
#include <string>
#include <mutex>
#include <iostream>
//#include <unistd.h>

//#include "Engine/ImGui/imgui.h"
#include <Engine/Library/Console.h>

using namespace Engine::Library;

std::vector<char*> history;
std::mutex loggingMutex;

// Specifiecs what kind of log level is printed out. 0 = disabled, 1 = Error, 2 = Warn, 3 = Info
int Console::printLevel = LOG_ERROR;
const char* Console::LOG_FILE = "output.log"; // Default logfile
bool initialized_file = false;

#define BUFFER_SIZE 1024

const std::vector<char*> Console::GetHistory() {
    return history;
}

void file_out(const char *str){
    if(!initialized_file){
        remove("output_prev.log");
        rename(Console::LOG_FILE, "output_prev.log");
        initialized_file = true;
    }
    auto f = fopen(Console::LOG_FILE, "a");
    fputs(str, f);
    fputc('\n',f);
    fclose(f);
}

void Console::SetPrintLevel(int printLevel) {
    Console::printLevel = printLevel;
}

void Console::Log(const char* format, ...) {
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start (args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    buffer[BUFFER_SIZE-1] = '\0';
    va_end(args);
    
    std::lock_guard<std::mutex> lock(loggingMutex);
    
    file_out(buffer);
    history.push_back(Strdup(buffer)); // Needs to be free()'d
    //scrollDown = true; // Force down scroll to show new message
}

void Console::LogInfo(const char* format, ...) {
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start (args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    buffer[BUFFER_SIZE-1] = '\0';
    va_end(args);

    // Move the whole string forward to make space for info text

    const char* info = "[INFO] ";
    char* newlog = (char*)malloc(strlen(buffer) + strlen(info)+1);
    strcpy(newlog, info);
    strcpy(newlog + strlen(info), buffer);
    
    std::lock_guard<std::mutex> lock(loggingMutex);
    
    file_out(newlog);
    history.push_back(newlog); // Needs to be free()'d
    if(printLevel >= 3) std::cout << newlog << std::endl;
    //scrollDown = true; // Force down scroll to show new message
}

void Console::LogError(const char* format, ...) {
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start (args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    buffer[BUFFER_SIZE-1] = '\0';
    va_end(args);

    // Move the whole string forward to make space for info text

    const char* info = "[ERROR] ";
    char* newlog = (char*)malloc(strlen(buffer) + strlen(info)+1);
    strcpy(newlog, info);
    strcpy(newlog + strlen(info), buffer);
    
    std::lock_guard<std::mutex> lock(loggingMutex);
    
    file_out(newlog);
    history.push_back(newlog); // Needs to be free()'d
    if(printLevel >=1) std::cerr << newlog << std::endl;
    //scrollDown = true; // Force down scroll to show new message
}

void Console::LogWarn(const char* format, ...) {
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start (args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    buffer[BUFFER_SIZE-1] = '\0';
    va_end(args);

    // Move the whole string forward to make space for info text

    const char* info = "[WARN] ";
    char* newlog = (char*)malloc(strlen(buffer) + strlen(info) +1);
    strcpy(newlog, info);
    strcpy(newlog + strlen(info), buffer);
    
    std::lock_guard<std::mutex> lock(loggingMutex);
    
    file_out(newlog);
    history.push_back(newlog); // Needs to be free()'d
    if(printLevel >= 2) std::cout << newlog << std::endl;
    //scrollDown = true; // Force down scroll to show new message
}

void Console::FatalError(const char* format, ...) {
    // TODO implement
}


void Console::ClearLogs() {
    
    std::lock_guard<std::mutex> lock(loggingMutex);
    
    for(int i=0; i < (int)history.size();i++)
        free(history[i]);

    history.clear();
    //scrollDown = true;
}

