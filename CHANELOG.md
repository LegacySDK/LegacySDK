# 0.0.1-alpha.1
- Welcome, LegacySDK!
- Allow people to change scene, mod loader, proxy, and so much more!!!
Examples:
```cpp
#include <legacysdk/legacysdk.hpp>

using namespace legacysdk::log;
using legacysdk::memory::binding::Scene;

$execute([]() {
    legacysdk::ui::registerPreNavigateHook([](legacysdk::ui::NavigateContext& ctx) -> bool {
        if (ctx.scene == static_cast<int>(Scene::SaveMessage)) {
            ctx.scene = static_cast<int>(Scene::MainMenu);
        }
        return true;
    });
});
```
