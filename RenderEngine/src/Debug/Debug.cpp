#include <algorithm>
#include <Engine/Debug.h>
#include <Engine/Library/Console.h>
#include <Engine/RenderEngine/imgui.h>
#include <Engine/Input/InputManager.h>
#include <Engine/Input/InputKeys.h>
#include <sstream>

using namespace Engine::Library;
using namespace Engine;
using namespace Engine::Input;

bool Engine::Debug::showDebug = false;
bool Debug::showDebugWindow = false;
std::map<std::string, std::function<void()>> Debug::debugRenderCallbacks;
std::map<std::string, DebugWindow> Debug::debugWindows;
bool prevKeyState = false;
bool scrollDown = false;
bool showWarnings = true;
bool showErrors = true;
bool showInfo = true;
bool showUntagged = true;
char inputBuffer[256];
int historyPos = -1;
std::map<std::string, std::function<bool(std::vector<std::string>)>> commands;
std::map<std::string, std::function<void()>> noParamCommands;
std::shared_ptr<Engine::Input::InputManager> Debug::inputManager;


void Debug::Init(std::shared_ptr<Engine::Input::InputManager> inputManager) {
    Debug::inputManager = inputManager;
    CreateDebugWindow("Debug console", Debug::RenderDebugConsole);

    RegisterCommandNoParams("clear", Console::ClearLogs);
}

void Debug::AddDebugRenderCallback(std::string name, std::function<void()> callback) {
    if(!callback) return; // Don't register if function is empty or non-existant
    debugRenderCallbacks.insert(std::pair(name, callback));
}

void Debug::CreateDebugWindow(std::string name, std::function<void()> callback) {
    if(!callback) return; // Same as above, Noteworthy: boost::function can't be compared to NULL or nullptr, check .empty() or cast to bool
    debugWindows[name] = {false, callback};
}

void Debug::RenderDebugWindows() {
    if(!showDebug || (debugRenderCallbacks.empty() && debugWindows.empty())) return;
    if(ImGui::BeginMainMenuBar()) {
        //if(ImGui::BeginMenu("Debug")) {
            if(!debugRenderCallbacks.empty()) {
                if (ImGui::MenuItem("Debug", "", showDebugWindow, true)) {
                    showDebugWindow = !showDebugWindow;
                }
            }
            for(auto const& it: debugWindows) {
                auto debugWindow = it.second;
                if(ImGui::MenuItem(it.first.c_str(), "", debugWindow.open, true)) {
                    debugWindows[it.first].open = !debugWindow.open;
                }
            }
           // ImGui::EndMenu();
        //}
        ImGui::EndMainMenuBar();
    }

    if(showDebugWindow && !debugRenderCallbacks.empty()) {
        ImGui::Begin("Debug", &showDebugWindow);
        for(auto const& callbackPair: debugRenderCallbacks) {
            callbackPair.second();
        }
        ImGui::End();
    }

    for(auto const& it: debugWindows) {
        auto debugWindow = it.second;
        if(!debugWindow.open) continue;

        ImGui::Begin(it.first.c_str(), &debugWindows[it.first].open, 0);
        debugWindow.callback();
        ImGui::End();
    }
}

void Debug::Update() {
    if (inputManager->IsKeyPressed(KEY_F1)) {
        showDebug = !showDebug;
        if(showDebug) {
            inputManager->ShowMouse();
        }
        else {
            inputManager->HideMouse();
        }
    }
}

void Debug::RemoveDebugRenderCallback(std::string name) {
    auto it = debugRenderCallbacks.find(name);
    if(it != debugRenderCallbacks.end()) {
        debugRenderCallbacks.erase(it);
    }
}

void Debug::RemoveDebugWindow(std::string name) {
    auto it = debugWindows.find(name);
    if(it != debugWindows.end()) {
        debugWindows.erase(it);
    }
}

int TextEditCallback(ImGuiInputTextCallbackData* data) {

    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
    {
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf)
        {
            const char c = word_start[-1];
            if (c == ' ' || c == '\t' || c == ',' || c == ';')
                break;
            word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (auto it = commands.begin(); it != commands.end(); it++)
            if (Strnicmp((*it).first.c_str(), word_start, (int)(word_end - word_start)) == 0)
                candidates.push_back((*it).first.c_str());

        for (auto it = noParamCommands.begin(); it != noParamCommands.end(); it++)
            if (Strnicmp((*it).first.c_str(), word_start, (int)(word_end - word_start)) == 0)
                candidates.push_back((*it).first.c_str());


        if (candidates.Size == 0)
        {
            // No match
            Console::Log("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
        }
        else if (candidates.Size == 1)
        {
            // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
            data->InsertChars(data->CursorPos, candidates[0]);
            data->InsertChars(data->CursorPos, " ");
        }
        else
        {
            // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
            int match_len = (int)(word_end - word_start);
            for (;;)
            {
                int c = 0;
                bool all_candidates_matches = true;
                for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                    if (i == 0)
                        c = toupper(candidates[i][match_len]);
                    else if (c == 0 || c != toupper(candidates[i][match_len]))
                        all_candidates_matches = false;
                if (!all_candidates_matches)
                    break;
                match_len++;
            }

            if (match_len > 0)
            {
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
            }

            // List matches
            Console::Log("Possible matches:\n");
            for (int i = 0; i < candidates.Size; i++)
                Console::Log("- %s\n", candidates[i]);
        }

        break;
    }
    case ImGuiInputTextFlags_CallbackHistory:
    {
        auto history = Console::GetHistory();
        // Example of HISTORY
        const int prev_history_pos = historyPos;
        if (data->EventKey == ImGuiKey_UpArrow)
        {
            if (historyPos == -1)
                historyPos = history.size() - 1;
            else if (historyPos > 0)
                historyPos--;
        }
        else if (data->EventKey == ImGuiKey_DownArrow)
        {
            if (historyPos != -1)
                if (++historyPos >= (int)history.size())
                    historyPos = -1;
        }

        // A better implementation would preserve the data on the current input line along with cursor position.
        if (prev_history_pos != historyPos)
        {
            const char* history_str = (historyPos >= 0) ? history[historyPos] : "";
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, history_str);
        }
    }
    }
    return 0;
}

bool Debug::IsDebugVisible() {
    return showDebug;
}

void Debug::RenderDebugConsole() {

    if (ImGui::SmallButton("Clear")) Console::ClearLogs();

    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    static ImGuiTextFilter filter;
    filter.Draw(R"(Filter ("incl,-excl") ("error"))", 180);
    ImGui::PopStyleVar(); ImGui::SameLine();
    ImGui::Checkbox("Show Untagged", &showUntagged); //ImGui::SameLine();
    ImGui::Checkbox("Show Info", &showInfo); ImGui::SameLine();
    ImGui::Checkbox("Show warnings", &showWarnings); ImGui::SameLine();
    ImGui::Checkbox("Show Errors", &showErrors);
    ImGui::Text("Press Tab for autocomplete");

    ImGui::Separator();

    // Make space for command input box
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    auto history = Console::GetHistory();
    for (auto i = 0; i < (int)history.size(); i++)
    {
        auto* item = history[i];
        if (!filter.PassFilter(item)) continue;

        ImVec4 col = ImColor(1.f, 1.f, 1.f, 1.f);
        if (strstr(item, "[ERROR]")) {
            if (!showErrors) continue;
            col = ImColor(1.f, .4f, .4f, 1.f); // If we see error text, color text red TODO: warning and info colors
        }
        else if (strstr(item, "[INFO]")) {
            if (!showInfo) continue;
            col = ImColor(.4f, .4f, 1.f, 1.f);
        }
        else if (strstr(item, "[WARN]")) {
            if (!showWarnings) continue;
            col = ImColor(1.f, 1.f, .4f, 1.f);
        }
        else if (!showUntagged) // If we get this far this item is untagged
            continue;

        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::TextUnformatted(item);
        ImGui::PopStyleColor();
    }
    if (scrollDown)
        ImGui::SetScrollHere(1.f);

    scrollDown = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::Separator();

    //
    // Command input
    //

    ImGui::PushItemWidth(-1);
    bool reclaim_focus = false;
    if (ImGui::InputText("", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallback, nullptr))
    {
        char* s = inputBuffer;
        Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy(s, "");
        reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

}

void Debug::RegisterCommand(std::string cmd, std::function<bool(std::vector<std::string>)> callback) {
    auto it = commands.find(cmd);
    if (it != commands.end())
        Console::LogWarn("Registering new command which overwrites old command %s", cmd.c_str());

    commands[cmd] = callback;
}

void Debug::RegisterCommandNoParams(std::string cmd, std::function<void()> callback) {
    auto it = noParamCommands.find(cmd);
    if (it != noParamCommands.end())
        Console::LogWarn("Registering new command which overwrites old command %s", cmd.c_str());

    noParamCommands[cmd] = callback;
}

void Debug::ExecCommand(char* cmdStr) {
    // TODO
    Console::Log("> %s", cmdStr);

    // String split on spaces and pass on as arguments
    std::vector<std::string> strings;
    std::istringstream f(cmdStr);
    std::string s, commandStr;
    while (getline(f, s, ' '))
    {
        if (strings.size() == 0 && commandStr == "")
            commandStr = s;
        else
            strings.push_back(s);
    }

    auto it = commands.find(commandStr);
    auto it2 = noParamCommands.find(commandStr);
    if (it == commands.end() && it2 == noParamCommands.end())
    {
        Console::Log("Unknown command");
    }
    else
    {
        // Call functor
        // Remove function call and pass on arguments
        auto cmd = std::string(cmdStr);

        auto pos = cmd.find(commandStr);
        if (pos != std::string::npos)
        {
            // Remove the command string itself and the space after it
            cmd.erase(pos, commandStr.size() + 1);
        }

        if (it == commands.end())
        {
            (*it2).second();
        }
        else
        {
            (*it).second(strings);
        }
    }
}