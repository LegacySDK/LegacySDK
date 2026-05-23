#include <windows.h>
#include <MinHook.h>

#include <terminal.hpp>
#include <modloader.hpp>
#include <crashlog.hpp>

/*
  * @brief Where it all started:tm:
  * @note doesnt really work well but eh, it works.
  * ill also maybe delete the messagebox later and replace it with crash log
*/

DWORD WINAPI SDKMAIN(LPVOID lpParam) {
    MessageBoxA(NULL,"text","LegacySDK",MB_OK | MB_ICONINFORMATION);

    modloader::loadMods();

    Sleep(300);
    termprintf("Crashing now!");
    crashlog::triggerCrash();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);

            CreateThread(NULL, 0, SDKMAIN, hModule, 0, NULL);

            // terminal
            terminit();
            termprintf("[LOADER]:   LegacySDK hooked + in dll main or whatever idk");
            break;
        case DLL_PROCESS_DETACH:
            modloader::unloadMods();
            break;
    }
    return TRUE;
}