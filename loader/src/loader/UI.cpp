#include <windows.h>
#include <MinHook.h>

#include <UIController/init.hpp>

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

class UIControllerVTable {
public:
    virtual ~UIControllerVTable() = default;

    virtual void NavigateToHomeMenu() = 0;

    virtual bool NavigateToScene(
        int iPad,
        int scene,
        void* initData,
        int layer,
        int group
    ) = 0;

    virtual bool NavigateBack(
        int iPad,
        bool forceUsePad,
        int scene,
        int layer
    ) = 0;

    virtual void CloseAllPlayersScenes() = 0;

    virtual void CloseUIScenes(
        int iPad,
        bool forceIPad
    ) = 0;

    virtual void* GetTopScene(
        int iPad,
        int layer,
        int group
    ) = 0;

    virtual bool IsSceneInStack(
        int iPad,
        int scene
    ) = 0;

    virtual void* FindScene(
        int sceneType
    ) = 0;
};

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

        legacysdk::ui::initUICONTROLLER(
            g_pUIController,

            [](void* self) {
                reinterpret_cast<UIControllerVTable*>(self)
                    ->NavigateToHomeMenu();
            },

            [](void* self,
               int iPad,
               int scene,
               void* initData,
               int layer,
               int group) {

                return reinterpret_cast<UIControllerVTable*>(self)
                    ->NavigateToScene(
                        iPad,
                        scene,
                        initData,
                        layer,
                        group
                    );
            },

            [](void* self,
               int iPad,
               bool forceUsePad,
               int scene,
               int layer) {

                return reinterpret_cast<UIControllerVTable*>(self)
                    ->NavigateBack(
                        iPad,
                        forceUsePad,
                        scene,
                        layer
                    );
            },

            [](void* self) {
                reinterpret_cast<UIControllerVTable*>(self)
                    ->CloseAllPlayersScenes();
            },

            [](void* self,
               int iPad,
               bool forceIPad) {

                reinterpret_cast<UIControllerVTable*>(self)
                    ->CloseUIScenes(
                        iPad,
                        forceIPad
                    );
            },

            [](void* self,
               int iPad,
               int layer,
               int group)
                -> legacysdk::ui::UIScene* {

                return reinterpret_cast<
                    legacysdk::ui::UIScene*
                >(
                    reinterpret_cast<UIControllerVTable*>(self)
                        ->GetTopScene(
                            iPad,
                            layer,
                            group
                        )
                );
            },

            [](void* self,
               int iPad,
               int scene) {

                return reinterpret_cast<UIControllerVTable*>(self)
                    ->IsSceneInStack(
                        iPad,
                        scene
                    );
            },

            [](void* self,
               int sceneType)
                -> legacysdk::ui::UIScene* {

                return reinterpret_cast<
                    legacysdk::ui::UIScene*
                >(
                    reinterpret_cast<UIControllerVTable*>(self)
                        ->FindScene(sceneType)
                );
            }
        );
    }

    return onavscene(
        self,
        iPad,
        scene,
        initData,
        layer,
        group
    );
}

void initUICONTROLLER() {

    uintptr_t base =
        reinterpret_cast<uintptr_t>(
            GetModuleHandleA(nullptr)
        );

    uintptr_t addr = base + 0x627190;

    MH_CreateHook(
        reinterpret_cast<void*>(addr),
        &hknavscene,
        reinterpret_cast<void**>(&onavscene)
    );

    MH_EnableHook(
        reinterpret_cast<void*>(addr)
    );
}