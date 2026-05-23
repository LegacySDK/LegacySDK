#pragma once
#include <string>

namespace legacysdk::log {
    __declspec(dllexport) void info(const char* msg);
    __declspec(dllexport) void warn(const char* msg);
}