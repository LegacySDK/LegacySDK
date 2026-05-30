#include <legacysdk/legacysdk.hpp>

using namespace legacysdk::log;
using legacysdk::memory::binding::Scene;

namespace {
    bool g_buttonRegistered = false;

    void createButton() {
        if (g_buttonRegistered) {
            return;
        }
        g_buttonRegistered = true;

        legacysdk::ui::mainmenu::btn modMenu{};
        modMenu.slot = legacysdk::ui::mainmenu::Slot::Leaderboards;
        modMenu.label = L"LegacySDK";
        modMenu.onPress = []() {
            legacysdk::ui::mainmenu::showMessage(
                legacysdk::strings::IDS_PLAY_GAME,
                legacysdk::strings::IDS_HINTS);
        };
        legacysdk::ui::mainmenu::addButton(std::move(modMenu));
    }
}

$execute([]() {
    info("noautosave mod loaded");
    
    legacysdk::ui::registerPreNavigateHook([](legacysdk::ui::NavigateContext& ctx) -> bool {
        if (ctx.scene == static_cast<int>(Scene::SaveMessage)) {
            ctx.scene = static_cast<int>(Scene::MainMenu);
        }
        return true;
    });
    legacysdk::ui::registerPostNavigateHook([](const legacysdk::ui::NavigateContext& ctx) {
        if (ctx.scene == static_cast<int>(Scene::MainMenu)) {
            createButton();
        }
    });
});
