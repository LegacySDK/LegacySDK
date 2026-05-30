#pragma once
#include <cstdint>
#include <windows.h>

namespace legacysdk::memory::binding {
    constexpr std::uintptr_t kImageBase = 0x140000000ULL;
    constexpr std::uintptr_t kNavigateToScene = 0x627190;

    enum class Scene : int {
        PartnernetPassword = 0,
        Intro = 1,
        SaveMessage = 2,
        MainMenu = 3,
        FullscreenProgress = 4,
        PauseMenu = 5,
    };

    enum class MainMenuControl : int {
        PlayGame = 0,
        Leaderboards = 1,
        Achievements = 2,
        HelpAndOptions = 3,
        UnlockOrDLC = 4,
        Exit = 5,
    };
    constexpr std::uintptr_t kUIControlButtonInit = 0;
    constexpr std::uintptr_t kMainMenuButtonsOffset = 0x348;
    constexpr std::size_t kUIControlButtonSize = 0x1A0;

    inline std::uintptr_t moduleBase() {
        return reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
    }

    inline void* rvaToPtr(std::uintptr_t rva) {
        return reinterpret_cast<void*>(moduleBase() + rva);
    }

}