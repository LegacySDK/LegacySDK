#include <legacysdk/ui/mainmenu.hpp>
#include <legacysdk/ui/messagebox.hpp>
#include <legacysdk/ui/hooks.hpp>
#include <legacysdk/memory/bindings.hpp>
#include <interface/UIController/UIControllerInterface.hpp>
#include <windows.h>
#include <vector>
#include <mutex>

namespace legacysdk::ui::mainmenu {
    namespace {
        struct RegisteredButton {
            btn config;
            int interceptScene;
        };

        std::mutex g_mutex;
        std::vector<RegisteredButton> g_buttons;
        bool g_hooksInstalled = false;
        bool g_welcomeEnabled = false;
        bool g_welcomeShown = false;
        unsigned int g_welcomeTitle = 623;
        unsigned int g_welcomeBody = 625;

        int defaultInterceptScene(Slot slot) {
            switch (slot) {
            case Slot::Leaderboards:
                return static_cast<int>(EUIScene::LeaderboardsMenu);
            case Slot::HelpAndOptions:
                return static_cast<int>(EUIScene::HelpAndOptionsMenu);
            case Slot::UnlockOrDLC:
                return static_cast<int>(EUIScene::DLCMainMenu);
            case Slot::PlayGame:
                return static_cast<int>(EUIScene::LoadOrJoinMenu);
            default:
                return -1;
            }
        }

        using InitButtonFn = void* (__fastcall*)(void* self, const wchar_t* label, int id);

        void applyButtonLabel(UIScene* scene, Slot slot, const std::wstring& label) {
            if (!scene || label.empty()) return;

            constexpr uintptr_t kOffset = legacysdk::memory::binding::kMainMenuButtonsOffset;
            constexpr uintptr_t kInitRva = legacysdk::memory::binding::kUIControlButtonInit;
            if (kOffset == 0 || kInitRva == 0) return;

            auto* mainMenu = reinterpret_cast<uint8_t*>(scene);
            auto* button = reinterpret_cast<void*>(
                mainMenu + kOffset + static_cast<size_t>(slot) * legacysdk::memory::binding::kUIControlButtonSize);

            auto init = reinterpret_cast<InitButtonFn>(legacysdk::memory::binding::rvaToPtr(kInitRva));
            if (init) {
                init(button, label.c_str(), static_cast<int>(slot));
            }
        }

        void applyAllLabels() {
            auto* ui = legacysdk::ui::getUIController();
            if (!ui) return;

            auto* scene = ui->findScene(EUIScene::MainMenu);
            if (!scene) return;

            std::lock_guard<std::mutex> lock(g_mutex);
            for (const auto& entry : g_buttons) {
                if (!entry.config.label.empty()) {
                    applyButtonLabel(scene, entry.config.slot, entry.config.label);
                }
            }
        }

        static unsigned int g_msgOpts[1] = {963};
        static mbox g_msgBox{};

        void showMessageInternal(unsigned int titleId, unsigned int bodyId) {
            auto* ui = legacysdk::ui::getUIController();
            if (!ui) return;

            g_msgOpts[0] = 963;
            g_msgBox = {};
            g_msgBox.uiTitle = titleId;
            g_msgBox.uiText = bodyId;
            g_msgBox.uiOptionA = g_msgOpts;
            g_msgBox.uiOptionC = 1;
            g_msgBox.dwFocusButton = 0;

            ui->navigateToScene(
                0,
                EUIScene::MessageBox,
                &g_msgBox,
                EUILayer::Alert,
                EUIGroup::Fullscreen);
        }

        DWORD WINAPI runCallbackThread(LPVOID param) {
            auto* fn = reinterpret_cast<std::function<void()>*>(param);
            if (fn && *fn) {
                (*fn)();
            }
            delete fn;
            return 0;
        }

        void queueCallback(std::function<void()> fn) {
            CreateThread(nullptr, 0, runCallbackThread, new std::function<void()>(std::move(fn)), 0, nullptr);
        }

        void onMainMenuShown() {
            applyAllLabels();

            if (g_welcomeEnabled && !g_welcomeShown) {
                g_welcomeShown = true;
                queueCallback([]() { showMessageInternal(g_welcomeTitle, g_welcomeBody); });
            }
        }

        bool onPreNavigate(NavigateContext& ctx) {
            std::function<void()> callback;
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                for (const auto& entry : g_buttons) {
                    if (!entry.config.onPress) continue;
                    if (ctx.scene != entry.interceptScene) continue;
                    callback = entry.config.onPress;
                    break;
                }
            }

            if (callback) {
                queueCallback(std::move(callback));
                return false;
            }
            return true;
        }

        void onPostNavigate(const NavigateContext& ctx) {
            if (ctx.scene == static_cast<int>(EUIScene::MainMenu)) {
                onMainMenuShown();
            }
        }

        void installHooks() {
            if (g_hooksInstalled) return;
            g_hooksInstalled = true;

            registerPreNavigateHook(onPreNavigate);
            registerPostNavigateHook(onPostNavigate);
        }
    }

    void addButton(btn button) {
        RegisteredButton entry;
        entry.config = std::move(button);
        entry.interceptScene = entry.config.interceptScene;
        if (entry.interceptScene < 0) {
            entry.interceptScene = defaultInterceptScene(entry.config.slot);
        }

        std::lock_guard<std::mutex> lock(g_mutex);
        g_buttons.push_back(std::move(entry));
        installHooks();
    }

    void showMessage(unsigned int titleStringId, unsigned int bodyStringId) {
        installHooks();
        showMessageInternal(titleStringId, bodyStringId);
    }

    void showMessageWide(const wchar_t* title, const wchar_t* body) {
        MessageBoxW(nullptr, body ? body : L"", title ? title : L"LegacySDK Mod", MB_OK | MB_ICONINFORMATION);
    }

    void setWelcomeMessage(unsigned int titleStringId, unsigned int bodyStringId) {
        g_welcomeTitle = titleStringId;
        g_welcomeBody = bodyStringId;
    }

    void enableWelcomeOnFirstVisit(bool enable) {
        g_welcomeEnabled = enable;
    }

    void init() {
        installHooks();
    }

}
