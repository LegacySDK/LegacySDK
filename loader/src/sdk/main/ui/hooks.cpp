#include <legacysdk/ui/hooks.hpp>
#include <vector>

namespace legacysdk::ui {
    static std::vector<PreNavigateHook> g_preHooks;
    static std::vector<PostNavigateHook> g_postHooks;

    void registerPreNavigateHook(PreNavigateHook hook) {
        g_preHooks.push_back(std::move(hook));
    }

    void registerPostNavigateHook(PostNavigateHook hook) {
        g_postHooks.push_back(std::move(hook));
    }

    bool dispatchPreNavigate(NavigateContext& ctx) {
        for (auto& hook : g_preHooks) {
            if (!hook(ctx)) {
                return false;
            }
        }
        return true;
    }

    void dispatchPostNavigate(const NavigateContext& ctx) {
        for (auto& hook : g_postHooks) {
            hook(ctx);
        }
    }
}
