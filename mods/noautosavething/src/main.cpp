#include <legacysdk/legacysdk.hpp>

using namespace legacysdk::log;
using legacysdk::memory::binding::Scene;

$execute([]() {
    info("noautosave mod loaded");

    legacysdk::ui::registerPreNavigateHook([](legacysdk::ui::NavigateContext& ctx) -> bool {
        if (ctx.scene == static_cast<int>(Scene::SaveMessage)) {
            info("redirecting SaveMessage to MainMenu (skip autosave prompt)");
            ctx.scene = static_cast<int>(Scene::MainMenu);
        }
        return true;
    });
});
