#include <Windows.h>
#include <string>
#include <array>
#include <vector>
#include <filesystem>


/*
 *@note ill replace urls with ones or github later, too lazy to bother
 */

struct XINPUT_STATE;
struct XINPUT_CAPABILITIES;
struct XINPUT_VIBRATION;

constexpr static auto MAX_PATH_CHARS = 32768u;

constexpr wchar_t REDIST_ERROR[] = L"Could not load LegacySDK!\n"
    "This is most likely due to an outdated redist package\n"
    "Do you want to download/update the Visual C++ Redistributable now?";

constexpr wchar_t NOTFOUND[] = L"Could not load LegacySDK!\n"
    "Please reinstall LegacySDK or contact me on discord if the problem persists (misko.bin <- discord)\n";

constexpr wchar_t BAD_EXE_FORMAT_ERROR[] = L"Could not load LegacySDK!\n"
    "Your installation may be corrupted.\n"
    "Open the download page?";

constexpr wchar_t ALT_REDIST_ERROR[] = L"Could not load LegacySDK!\n\n"
    "Please **delete** the following files from your \\x64\\Debug (or Release) directory and try again: ";

static HMODULE getXInput() {
    static auto xinput = []() -> HMODULE {
        std::wstring path(MAX_PATH_CHARS, L'\0');
        auto size = GetSystemDirectoryW(path.data(), path.size());
        if (size) {
            path.resize(size);
            return LoadLibraryW((path + L"\\XINPUT9_1_0.dll").c_str());
        }
        return NULL;
    }();
    return xinput;
}

static FARPROC getFP(const std::string& sym) {
    if (auto xinput = getXInput())
        return GetProcAddress(xinput, sym.c_str());

    return NULL;
}

#pragma comment(linker, "/export:XInputGetState,@2")
extern "C" DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState) {
    static auto fp = getFP("XInputGetState");
    if (fp) {
        using FPType = decltype(&XInputGetState);
        return reinterpret_cast<FPType>(fp)(dwUserIndex, pState);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

#pragma comment(linker, "/export:XInputSetState,@3")
extern "C" DWORD XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {
    static auto fp = getFP("XInputSetState");
    if (fp) {
        using FPType = decltype(&XInputSetState);
        return reinterpret_cast<FPType>(fp)(dwUserIndex, pVibration);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

#pragma comment(linker, "/export:XInputGetCapabilities,@4")
extern "C" DWORD XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities) {
    static auto fp = getFP("XInputGetCapabilities");
    if (fp) {
        using FPType = decltype(&XInputGetCapabilities);
        return reinterpret_cast<FPType>(fp)(dwUserIndex, dwFlags, pCapabilities);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

static std::wstring getErrorString(DWORD error) {
    return L"Error code: " + std::to_wstring(error);
}

static DWORD errorThread(LPVOID param) {
    const DWORD error = reinterpret_cast<DWORD64>(param);

    if (error == ERROR_DLL_INIT_FAILED) {

        std::array<std::wstring, 4> msvcpDlls = {
            L"msvcp140.dll",
            L"msvcp140d.dll",
            L"vcruntime140.dll",
            L"vcruntime140d.dll"
        };

        std::vector<std::wstring> foundDlls;
        for(auto dll : msvcpDlls) {
            if(std::filesystem::exists(dll)) {
                foundDlls.push_back(dll);
            }
        }

        if(foundDlls.empty()) {
            const auto choice = MessageBoxW(NULL, REDIST_ERROR, L"Load failed", MB_YESNO | MB_ICONWARNING);
            if (choice == IDYES) {
                ShellExecuteW(NULL, L"open", L"https://example.com", NULL, NULL, SW_SHOWNORMAL);
                ExitProcess(0);
            }
        } else {
            std::wstring files = ALT_REDIST_ERROR;
            bool first = true;
            for(auto dll : foundDlls) {
                if(!first) files += L", ";
                files += dll;
                first = false;
            }
            const auto choice = MessageBoxW(NULL, files.c_str(), L"Load failed", MB_OK | MB_ICONWARNING);
        }

    } else if (error == ERROR_MOD_NOT_FOUND) {
        if(!std::filesystem::exists(L"legacysdk.dll")) {
            if(MessageBoxW(NULL, NOTFOUND, L"Load failed (error code: 126)", MB_YESNO | MB_ICONWARNING) == IDYES) {
                ShellExecuteW(NULL, L"open", L"https://example.com", NULL, NULL, SW_SHOWNORMAL);
                ExitProcess(0);
            }
        } else {
            if(MessageBoxW(NULL, REDIST_ERROR, L"Load failed (error code: 126)", MB_YESNO | MB_ICONWARNING) == IDYES) {
                ShellExecuteW(NULL, L"open", L"https://example.com", NULL, NULL, SW_SHOWNORMAL);
                ExitProcess(0);
            }
        }
    } else if (error == ERROR_BAD_EXE_FORMAT) {
        if(MessageBoxW(NULL, BAD_EXE_FORMAT_ERROR, L"Load failed (error code: 193)", MB_YESNO | MB_ICONWARNING) == IDYES) {
            ShellExecuteW(NULL, L"open", L"https://example.com", NULL, NULL, SW_SHOWNORMAL);
            ExitProcess(0);
        }
    } else if (error == 4551) {
        MessageBoxW(NULL, L"disable smartapp control in windows defender since its blocking the loading of legacysdk", L"Load failed (error code: 4551)", MB_OK | MB_ICONWARNING);
    } else {
        MessageBoxW(NULL, getErrorString(error).c_str(), L"Load failed" , MB_OK | MB_ICONWARNING);
    }

    return 0u;
}

bool isRedistUpdated() {
    HMODULE hMod = GetModuleHandleW(L"vcruntime140.dll");
    if(!hMod) return true;

    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(hMod, szPath, MAX_PATH);

    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSizeW(szPath, &dummy);
    if (dwSize == 0) return true;

    std::vector<BYTE> data(dwSize);
    if (!GetFileVersionInfoW(szPath, 0, dwSize, data.data())) return true;

    VS_FIXEDFILEINFO* pFileInfo = nullptr;
    UINT len = 0;
    if (!VerQueryValueW(data.data(), L"\\", (LPVOID*)&pFileInfo, &len)) return true;

    return HIWORD(pFileInfo->dwFileVersionMS) == 14 && LOWORD(pFileInfo->dwFileVersionMS) >= 44;
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        if (!isRedistUpdated()) {
            if (MessageBoxW(NULL, REDIST_ERROR, L"Warning", MB_YESNO | MB_ICONWARNING) == IDYES) {
                ShellExecuteW(NULL, L"open", L"https://example.com", NULL, NULL, SW_SHOWNORMAL);
                ExitProcess(0);
            }
        }
        if (LoadLibraryW(L"legacysdk.dll") == NULL) {
            const auto param = reinterpret_cast<LPVOID>(static_cast<DWORD64>(GetLastError()));
            CreateThread(NULL, 0, &errorThread, param, 0, NULL);
        }
    }

    return TRUE;
}