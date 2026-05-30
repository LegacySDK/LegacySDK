#pragma once
#include <functional>
#include <string>

namespace legacysdk::ui::mainmenu {
    enum class Slot : int {
        PlayGame = 0,
        Leaderboards = 1,
        Achievements = 2,
        HelpAndOptions = 3,
        UnlockOrDLC = 4,
        Exit = 5,
    };
    struct btn {
        Slot slot = Slot::Leaderboards;
        std::wstring label = L"Hey, Please check if your mod is working since I am just a placeholder";
        int interceptScene = -1;
        std::function<void()> onPress;
    };

    __declspec(dllexport) void addButton(btn button);
    __declspec(dllexport) void showMessage(unsigned int titleStringId, unsigned int bodyStringId);
    __declspec(dllexport) void showMessageWide(const wchar_t* title, const wchar_t* body);
    __declspec(dllexport) void setWelcomeMessage(unsigned int titleStringId, unsigned int bodyStringId);
    __declspec(dllexport) void enableWelcomeOnFirstVisit(bool enable);

    __declspec(dllexport) void init();

}