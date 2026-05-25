#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstdint>

namespace crashmanager {
    enum CRASH {
        CRASH_UNKNOWN,
        CRASH_ACCESS_VIOLATION,
        CRASH_STACK_OVERFLOW,
        CRASH_DIVIDE_BY_ZERO,
        CRASH_ILLEGAL_INSTRUCTION,
        CRASH_FLOAT_DENORMAL,
        CRASH_FLOAT_DIVIDE_BY_ZERO,
        CRASH_FLOAT_OVERFLOW,
        CRASH_FLOAT_UNDERFLOW,
        CRASH_INT_OVERFLOW,
        CRASH_PRIVILEGED_INSTRUCTION,
        CRASH_BREAKPOINT,
        CRASH_MOD_RELATED
    };

    struct INFO {
        CRASH type;
        DWORD exceptionCode;
        DWORD64 exceptionAddress;
        uint32_t faultingModule;
        std::string faultingModPath;
        bool isModRelated;
        std::string description;
    };

    struct MI {
        DWORD64 baseAddress;
        DWORD64 endAddress;
        std::string path;
        std::string name;
        bool isSystemModule;
    };

    CRASH analyzeCrash(DWORD exceptionCode);
    bool isSystemModuleCrash(DWORD64 crashAddress, const std::vector<MI>& modules);
    INFO gatherCrashInfo(PEXCEPTION_POINTERS pExceptionInfo);
    std::string describeCrash(const INFO& info);
    bool isModuleSystem(const std::string& modulePath);
    void loadModuleList(std::vector<MI>& modules);
}
