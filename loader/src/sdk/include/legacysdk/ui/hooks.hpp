#pragma once
#include <functional>

namespace legacysdk::ui {
    struct NavigateContext {
        int iPad;
        int scene;
        void* initData;
        int layer;
        int group;
    };

    using PreNavigateHook = std::function<bool(NavigateContext& ctx)>;

    using PostNavigateHook = std::function<void(const NavigateContext& ctx)>;

    __declspec(dllexport) void registerPreNavigateHook(PreNavigateHook hook);
    __declspec(dllexport) void registerPostNavigateHook(PostNavigateHook hook);

    __declspec(dllexport) bool dispatchPreNavigate(NavigateContext& ctx);
    __declspec(dllexport) void dispatchPostNavigate(const NavigateContext& ctx);
    __declspec(dllexport) bool isUiReady();
    __declspec(dllexport) void notifyUiReady();
}