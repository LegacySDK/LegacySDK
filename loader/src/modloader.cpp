#include <modloader.hpp>
#include <terminal.hpp>
#include <filesystem>
#include <mod.hpp>

namespace modloader {
    static std::vector<LoadedMod> g_loadedMods;
    void loadMods() {
        std::string modsPath = "mods";
    
        if (!std::filesystem::exists(modsPath)) {
            termprintf("[MODLOADER]: folder !found, creating");
            std::filesystem::create_directory(modsPath);
            return;
        }

        termprintf("[MODLOADER]: scanning");

        for (const auto& entry : std::filesystem::directory_iterator(modsPath)) {
            if (entry.path().extension() == ".dll") {
                std::string dllPath = entry.path().string();
                std::string dllName = entry.path().stem().string();
            
                termprintf("[MODLOADER]: loading mod: %s", dllName.c_str());
            
                HMODULE hMod = LoadLibraryA(dllPath.c_str());
                if (hMod) {
                    LoadedMod mod;
                    mod.handle = hMod;
                    mod.path = dllPath;
                    mod.name = dllName;
                    g_loadedMods.push_back(mod);

                    termprintf("[MODLOADER]: successfully loaded mod: %s", dllName.c_str());
                } else {
                    termprintf("[MODLOADER]: failed to load mod: %s (error: %lu)", dllName.c_str(), GetLastError());
                }
            }
        }
    
        termprintf("[MODLOADER]: loaded %zu mod(s)", g_loadedMods.size());
    
        // mrbeast
        using namespace legacysdk::mod;
        dispatchEvent(me::Init);
        dispatchEvent(me::Load);
        dispatchEvent(me::Enable);
    }

    void unloadMods() {
        termprintf("[MODLOADER]: unloading mods...");
    
        for (auto& mod : g_loadedMods) {
            termprintf("[MODLOADER]: unloading mod: %s", mod.name.c_str());
            FreeLibrary(mod.handle);
        }

        g_loadedMods.clear();
        termprintf("[MODLOADER]: all mods unloaded");
    }
    const std::vector<LoadedMod>& getLoadedMods() {
        return g_loadedMods;
    }
}