#include <interface/UIController/UIControllerInterface.hpp>
#include <UIController/init.hpp>
#include <legacysdk/memory/bindings.hpp>
#include <windows.h>

namespace legacysdk::ui {
    class UIScene {};

    using FnNavigateToHomeMenu = void(__fastcall*)(void* self);
    using FnNavigateToScene = bool(__fastcall*)(void* self, int iPad, int scene, void* initData, int layer, int group);
    using FnNavigateBack = bool(__fastcall*)(void* self, int iPad, bool forceUsePad, int scene, int layer);
    using FnCloseAllPlayersScenes = void(__fastcall*)(void* self);
    using FnCloseUIScenes = void(__fastcall*)(void* self, int iPad, bool forceIPad);
    using FnGetTopScene = UIScene*(__fastcall*)(void* self, int iPad, int layer, int group);
    using FnIsSceneInStack = bool(__fastcall*)(void* self, int iPad, int scene);
    using FnFindScene = UIScene*(__fastcall*)(void* self, int sceneType);

    static void* g_pMinecraftUIController = nullptr;
    static FnNavigateToScene g_pNavigateToScene = nullptr;

    template<typename Fn>
    static Fn gameFn(std::uintptr_t rva) {
        return reinterpret_cast<Fn>(legacysdk::memory::binding::rvaToPtr(rva));
    }

    class UIControl : public IUIController {
    public:
        void navigateToHomeMenu() override {
            if (!g_pMinecraftUIController) return;
            if (legacysdk::memory::binding::kNavigateToHomeMenu == 0) return;
            gameFn<FnNavigateToHomeMenu>(legacysdk::memory::binding::kNavigateToHomeMenu)(g_pMinecraftUIController);
        }

        bool navigateToScene(int iPad, EUIScene scene, void* initData, EUILayer layer, EUIGroup group) override {
            if (!g_pNavigateToScene || !g_pMinecraftUIController) return false;
            return g_pNavigateToScene(
                g_pMinecraftUIController,
                iPad,
                static_cast<int>(scene),
                initData,
                static_cast<int>(layer),
                static_cast<int>(group));
        }

        bool navigateBack(int iPad, bool forceUsePad, EUIScene scene, EUILayer layer) override {
            if (!g_pMinecraftUIController || legacysdk::memory::binding::kNavigateBack == 0) return false;
            return gameFn<FnNavigateBack>(legacysdk::memory::binding::kNavigateBack)(
                g_pMinecraftUIController,
                iPad,
                forceUsePad,
                static_cast<int>(scene),
                static_cast<int>(layer));
        }

        void closeAllPlayersScenes() override {
            if (!g_pMinecraftUIController || legacysdk::memory::binding::kCloseAllPlayersScenes == 0) return;
            gameFn<FnCloseAllPlayersScenes>(legacysdk::memory::binding::kCloseAllPlayersScenes)(g_pMinecraftUIController);
        }

        void closeUIScenes(int iPad, bool forceIPad) override {
            (void)iPad;
            (void)forceIPad;
        }

        UIScene* getTopScene(int iPad, EUILayer layer, EUIGroup group) override {
            if (!g_pMinecraftUIController || legacysdk::memory::binding::kGetTopScene == 0) return nullptr;
            return gameFn<FnGetTopScene>(legacysdk::memory::binding::kGetTopScene)(
                g_pMinecraftUIController,
                iPad,
                static_cast<int>(layer),
                static_cast<int>(group));
        }

        bool isSceneInStack(int iPad, EUIScene scene) override {
            if (!g_pMinecraftUIController || legacysdk::memory::binding::kIsSceneInStack == 0) return false;
            return gameFn<FnIsSceneInStack>(legacysdk::memory::binding::kIsSceneInStack)(
                g_pMinecraftUIController,
                iPad,
                static_cast<int>(scene));
        }

        UIScene* findScene(EUIScene sceneType) override {
            if (!g_pMinecraftUIController) return nullptr;

            if (legacysdk::memory::binding::kFindScene != 0) {
                auto* scene = gameFn<FnFindScene>(legacysdk::memory::binding::kFindScene)(
                    g_pMinecraftUIController,
                    static_cast<int>(sceneType));
                if (legacysdk::memory::binding::isLikelyPointer(scene)) {
                    return scene;
                }
            }

            if (sceneType == EUIScene::MainMenu) {
                return getTopScene(0, EUILayer::Fullscreen, EUIGroup::Fullscreen);
            }

            return nullptr;
        }
    };

    static UIControl g_uiController;

    void initUICONTROLLER(void* controller) {
        g_pMinecraftUIController = controller;
    }

    void bindNavigateToScene(FuncNavigateToScene fn) {
        g_pNavigateToScene = fn;
    }

    IUIController* getUIController() {
        return &g_uiController;
    }
}
