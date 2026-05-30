#include <legacysdk/ui/hooks.hpp>
#include <mod.hpp>
#include <vector>

namespace legacysdk::ui {

static std::vector<PreNavigateHook> g_preHooks;
static std::vector<PostNavigateHook> g_postHooks;
static std::vector<PreNavigateHook> g_pendingPreHooks;
static std::vector<PostNavigateHook> g_pendingPostHooks;
static bool g_uiReady = false;
static int g_dispatchDepth = 0;

static void flushPendingHooks() {
    for (auto& hook : g_pendingPreHooks) {
        g_preHooks.push_back(std::move(hook));
    }
    g_pendingPreHooks.clear();

    for (auto& hook : g_pendingPostHooks) {
        g_postHooks.push_back(std::move(hook));
    }
    g_pendingPostHooks.clear();
}

void registerPreNavigateHook(PreNavigateHook hook) {
    if (g_dispatchDepth > 0) {
        g_pendingPreHooks.push_back(std::move(hook));
        return;
    }
    g_preHooks.push_back(std::move(hook));
}

void registerPostNavigateHook(PostNavigateHook hook) {
    if (g_dispatchDepth > 0) {
        g_pendingPostHooks.push_back(std::move(hook));
        return;
    }
    g_postHooks.push_back(std::move(hook));
}

bool isUiReady() {
    return g_uiReady;
}

void notifyUiReady() {
    if (g_uiReady) {
        return;
    }
    g_uiReady = true;
    legacysdk::mod::dispatchEvent(legacysdk::mod::me::Enable);
}

bool dispatchPreNavigate(NavigateContext& ctx) {
    if (!g_uiReady) {
        return true;
    }

    ++g_dispatchDepth;
    bool allow = true;
    for (auto& hook : g_preHooks) {
        if (!hook(ctx)) {
            allow = false;
            break;
        }
    }
    --g_dispatchDepth;

    if (g_dispatchDepth == 0) {
        flushPendingHooks();
    }

    return allow;
}

void dispatchPostNavigate(const NavigateContext& ctx) {
    if (!g_uiReady) {
        return;
    }

    ++g_dispatchDepth;
    for (auto& hook : g_postHooks) {
        hook(ctx);
    }
    --g_dispatchDepth;

    if (g_dispatchDepth == 0) {
        flushPendingHooks();
    }
}

} // namespace legacysdk::ui
