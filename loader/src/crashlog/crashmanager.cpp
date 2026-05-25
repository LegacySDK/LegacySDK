#include <crashmanager.hpp>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <tlhelp32.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

namespace crashmanager {
    CRASH analyzeCrash(DWORD exceptionCode) {
        switch (exceptionCode) {
            case EXCEPTION_ACCESS_VIOLATION:
                return CRASH_ACCESS_VIOLATION;
            case EXCEPTION_STACK_OVERFLOW:
                return CRASH_STACK_OVERFLOW;
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                return CRASH_DIVIDE_BY_ZERO;
            case EXCEPTION_ILLEGAL_INSTRUCTION:
                return CRASH_ILLEGAL_INSTRUCTION;
            case EXCEPTION_FLT_DENORMAL_OPERAND:
                return CRASH_FLOAT_DENORMAL;
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                return CRASH_FLOAT_DIVIDE_BY_ZERO;
            case EXCEPTION_FLT_OVERFLOW:
                return CRASH_FLOAT_OVERFLOW;
            case EXCEPTION_FLT_UNDERFLOW:
                return CRASH_FLOAT_UNDERFLOW;
            case EXCEPTION_INT_OVERFLOW:
                return CRASH_INT_OVERFLOW;
            case EXCEPTION_PRIV_INSTRUCTION:
                return CRASH_PRIVILEGED_INSTRUCTION;
            case EXCEPTION_BREAKPOINT:
                return CRASH_BREAKPOINT;
            default:
                return CRASH_UNKNOWN;
        }
    }

    bool isModuleSystem(const std::string& modulePath) {
        std::string lower = modulePath;
        std::transform(lower.begin(), lower.end(), lower.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (lower.find("windows") != std::string::npos ||
            lower.find("system32") != std::string::npos ||
            lower.find("syswow64") != std::string::npos ||
            lower.find("drivers") != std::string::npos) {
            return true;
        }

        const char* sysModules[] = {
            "kernel32", "ntdll", "msvcrt", "advapi32", "user32", "gdi32",
            "ws2_32", "wininet", "ole32", "shell32", "comctl32", "comdlg32",
            "shcore", "bcryptprimitives", "ucrtbase", "ntoskrnl", "hal",
            "wintrust", "crypt32", "sechost", "lsass", "csrss", "svchost"
        };

        for (const auto& mod : sysModules) {
            if (lower.find(mod) != std::string::npos) {
                return true;
            }
        }

        return false;
    }

    void loadModuleList(std::vector<MI>& modules) {
        HANDLE hCurrentProcess = GetCurrentProcess();
        HMODULE hMods[256];
        DWORD cbNeeded = 0;

        if (EnumProcessModules(hCurrentProcess, hMods, sizeof(hMods), &cbNeeded)) {
            DWORD moduleCount = cbNeeded / sizeof(HMODULE);
            
            for (DWORD i = 0; i < moduleCount && i < 256; i++) {
                MI modInfo;
                char szModName[MAX_PATH];
                char szModPath[MAX_PATH];

                if (GetModuleBaseNameA(hCurrentProcess, hMods[i], szModName, sizeof(szModName))) {
                    modInfo.name = szModName;
                }

                if (GetModuleFileNameExA(hCurrentProcess, hMods[i], szModPath, sizeof(szModPath))) {
                    modInfo.path = szModPath;
                }

                MODULEINFO modInfoStruct;
                if (GetModuleInformation(hCurrentProcess, hMods[i], &modInfoStruct, sizeof(modInfoStruct))) {
                    modInfo.baseAddress = reinterpret_cast<DWORD64>(modInfoStruct.lpBaseOfDll);
                    modInfo.endAddress = modInfo.baseAddress + modInfoStruct.SizeOfImage;
                }

                modInfo.isSystemModule = isModuleSystem(modInfo.path);
                modules.push_back(modInfo);
            }
        }
    }

    bool isSystemModuleCrash(DWORD64 crashAddress, const std::vector<MI>& modules) {
        for (const auto& mod : modules) {
            if (crashAddress >= mod.baseAddress && crashAddress < mod.endAddress) {
                return mod.isSystemModule;
            }
        }
        return false;
    }

    INFO gatherCrashInfo(PEXCEPTION_POINTERS pExceptionInfo) {
        INFO info = {};

        if (!pExceptionInfo || !pExceptionInfo->ExceptionRecord) {
            info.type = CRASH_UNKNOWN;
            info.exceptionCode = 0;
            return info;
        }

        info.exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
        info.exceptionAddress = reinterpret_cast<DWORD64>(pExceptionInfo->ExceptionRecord->ExceptionAddress);
        info.type = analyzeCrash(info.exceptionCode);

        std::vector<MI> modules;
        loadModuleList(modules);

        bool isSysModuleCrash = isSystemModuleCrash(info.exceptionAddress, modules);

        for (const auto& mod : modules) {
            if (info.exceptionAddress >= mod.baseAddress && 
                info.exceptionAddress < mod.endAddress) {
                info.faultingModule = static_cast<uint32_t>(info.exceptionAddress - mod.baseAddress);
                info.faultingModPath = mod.path;
                info.isModRelated = !mod.isSystemModule;
                break;
            }
        }

        if (isSysModuleCrash && !info.isModRelated) {
            info.type = CRASH_UNKNOWN;
        }

        return info;
    }

    std::string describeCrash(const INFO& info) {
        std::ostringstream oss;

        switch (info.type) {
            case CRASH_ACCESS_VIOLATION:
                oss << "Access Violation (0x" << std::hex << info.exceptionCode << std::dec << ")";
                break;
            case CRASH_STACK_OVERFLOW:
                oss << "Stack Overflow";
                break;
            case CRASH_DIVIDE_BY_ZERO:
                oss << "Integer Divide By Zero";
                break;
            case CRASH_ILLEGAL_INSTRUCTION:
                oss << "Illegal Instruction";
                break;
            case CRASH_FLOAT_DENORMAL:
                oss << "Float Denormal Operand";
                break;
            case CRASH_FLOAT_DIVIDE_BY_ZERO:
                oss << "Float Divide By Zero";
                break;
            case CRASH_FLOAT_OVERFLOW:
                oss << "Float Overflow";
                break;
            case CRASH_FLOAT_UNDERFLOW:
                oss << "Float Underflow";
                break;
            case CRASH_INT_OVERFLOW:
                oss << "Integer Overflow";
                break;
            case CRASH_PRIVILEGED_INSTRUCTION:
                oss << "Privileged Instruction";
                break;
            case CRASH_BREAKPOINT:
                oss << "Breakpoint Exception";
                break;
            default:
                oss << "Unknown Exception (0x" << std::hex << info.exceptionCode << std::dec << ")";
                break;
        }

        return oss.str();
    }
}
