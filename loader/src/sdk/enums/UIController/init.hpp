#pragma once

namespace legacysdk::ui {
    class UIScene;

    using FuncNavigateToScene = bool(__fastcall*)(void*, int, int, void*, int, int);

    __declspec(dllexport) void initUICONTROLLER(void* controller);
    __declspec(dllexport) void bindNavigateToScene(FuncNavigateToScene fn);
}
