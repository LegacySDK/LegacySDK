#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace modloader {
    struct LoadedMod {
        HMODULE handle;
        std::string path;
        std::string name;
    };

    void loadMods();
    void unloadMods();
    const std::vector<LoadedMod>& getLoadedMods();
}