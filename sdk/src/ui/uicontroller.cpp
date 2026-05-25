#include "../../enums/interface/UIController/UIControllerinterface.hpp"
#include <windows.h>

namespace legacysdk::ui {
	class UIScene {};

	typedef void(*FuncNavigateToHomeMenu)(void*);
	typedef bool(*FuncNavigateToScene)(void*, int, int, void*, int, int);
	typedef bool(*FuncNavigateBack)(void*, int, bool, int, int);
	typedef void(*FuncCloseAllPlayersScenes)(void*);
	typedef void(*FuncCloseUIScenes)(void*, int, bool);
	typedef UIScene*(*FuncGetTopScene)(void*, int, int, int);
	typedef bool(*FuncIsSceneInStack)(void*, int, int);
	typedef UIScene*(*FuncFindScene)(void*, int);

	static void* g_pMinecraftUIController = nullptr;
	static FuncNavigateToHomeMenu g_pNavigateToHomeMenu = nullptr;
	static FuncNavigateToScene g_pNavigateToScene = nullptr;
	static FuncNavigateBack g_pNavigateBack = nullptr;
	static FuncCloseAllPlayersScenes g_pCloseAllPlayersScenes = nullptr;
	static FuncCloseUIScenes g_pCloseUIScenes = nullptr;
	static FuncGetTopScene g_pGetTopScene = nullptr;
	static FuncIsSceneInStack g_pIsSceneInStack = nullptr;
	static FuncFindScene g_pFindScene = nullptr;

	class UIControl : public IUIController {
	public:
		void navigateToHomeMenu() override {
			if (g_pNavigateToHomeMenu && g_pMinecraftUIController) {
				g_pNavigateToHomeMenu(g_pMinecraftUIController);
			}
		}

		bool navigateToScene(int iPad, EUIScene scene, void* initData, EUILayer layer, EUIGroup group) override {
			if (g_pNavigateToScene && g_pMinecraftUIController) {
				return g_pNavigateToScene(g_pMinecraftUIController, iPad, (int)scene, initData, (int)layer, (int)group);
			}
			return false;
		}

		bool navigateBack(int iPad, bool forceUsePad, EUIScene scene, EUILayer layer) override {
			if (g_pNavigateBack && g_pMinecraftUIController) {
				return g_pNavigateBack(g_pMinecraftUIController, iPad, forceUsePad, (int)scene, (int)layer);
			}
			return false;
		}

		void closeAllPlayersScenes() override {
			if (g_pCloseAllPlayersScenes && g_pMinecraftUIController) {
				g_pCloseAllPlayersScenes(g_pMinecraftUIController);
			}
		}

		void closeUIScenes(int iPad, bool forceIPad) override {
			if (g_pCloseUIScenes && g_pMinecraftUIController) {
				g_pCloseUIScenes(g_pMinecraftUIController, iPad, forceIPad);
			}
		}

		UIScene* getTopScene(int iPad, EUILayer layer, EUIGroup group) override {
			if (g_pGetTopScene && g_pMinecraftUIController) {
				return g_pGetTopScene(g_pMinecraftUIController, iPad, (int)layer, (int)group);
			}
			return nullptr;
		}

		bool isSceneInStack(int iPad, EUIScene scene) override {
			if (g_pIsSceneInStack && g_pMinecraftUIController) {
				return g_pIsSceneInStack(g_pMinecraftUIController, iPad, (int)scene);
			}
			return false;
		}

		UIScene* findScene(EUIScene sceneType) override {
			if (g_pFindScene && g_pMinecraftUIController) {
				return g_pFindScene(g_pMinecraftUIController, (int)sceneType);
			}
			return nullptr;
		}
	};

	static UIControl g_uiController;

	void initUICONTROLLER(void* controller,
		FuncNavigateToHomeMenu pNavToHome,
		FuncNavigateToScene pNavToScene,
		FuncNavigateBack pNavBack,
		FuncCloseAllPlayersScenes pCloseAll,
		FuncCloseUIScenes pCloseUI,
		FuncGetTopScene pGetTop,
		FuncIsSceneInStack pIsInStack,
		FuncFindScene pFindScene) {

		g_pMinecraftUIController = controller;
		g_pNavigateToHomeMenu = pNavToHome;
		g_pNavigateToScene = pNavToScene;
		g_pNavigateBack = pNavBack;
		g_pCloseAllPlayersScenes = pCloseAll;
		g_pCloseUIScenes = pCloseUI;
		g_pGetTopScene = pGetTop;
		g_pIsSceneInStack = pIsInStack;
		g_pFindScene = pFindScene;
	}

	IUIController* getUIController() {
		return &g_uiController;
	}
}