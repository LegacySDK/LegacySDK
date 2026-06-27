#include <windows.h>
#include <MinHook.h>

#include <UIController/init.hpp>
#include <legacysdk/ui/hooks.hpp>

void* g_pUIController = nullptr;

typedef bool(__fastcall* navscene_t)(
    void* self,
    int iPad,
    int scene,
    void* initData,
    int layer,
    int group
);

static navscene_t onavscene = nullptr;

bool __fastcall hknavscene(
    void* self,
    int iPad,
    int scene,
    void* initData,
    int layer,
    int group
) {
    if (!g_pUIController) {
        g_pUIController = self;

        legacysdk::ui::notifyUiReady();
        legacysdk::ui::initUICONTROLLER(g_pUIController);
        legacysdk::ui::bindNavigateToScene(onavscene);
    }

    legacysdk::ui::NavigateContext ctx{iPad, scene, initData, layer, group};
    if (!legacysdk::ui::dispatchPreNavigate(ctx)) {
        return false;
    }

    const bool ok = onavscene(
        self,
        ctx.iPad,
        ctx.scene,
        ctx.initData,
        ctx.layer,
        ctx.group
    );

    if (ok) {
        legacysdk::ui::dispatchPostNavigate(ctx);
    }

    return ok;
}

void initUICONTROLLER() {
    uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    uintptr_t addr = base + 0x627190;

    MH_CreateHook(
        reinterpret_cast<void*>(addr),
        &hknavscene,
        reinterpret_cast<void**>(&onavscene)
    );

    MH_EnableHook(reinterpret_cast<void*>(addr));
}
