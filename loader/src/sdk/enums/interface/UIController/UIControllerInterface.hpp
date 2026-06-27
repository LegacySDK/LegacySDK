#pragma once
#include "../../enums/UIController/UIController.hpp"
namespace legacysdk::ui {
    class UIScene;

    class IUIController {
    public:
        virtual ~IUIController() = default;

        virtual void navigateToHomeMenu() = 0;
        virtual bool navigateToScene(int iPad, EUIScene scene, void* initData = nullptr, 
            EUILayer layer = EUILayer::Scene, EUIGroup group = EUIGroup::PAD) = 0;
        virtual bool navigateBack(int iPad, bool forceUsePad = false, EUIScene scene = EUIScene::COUNT, 
            EUILayer layer = EUILayer::COUNT) = 0;

        virtual void closeAllPlayersScenes() = 0;
        virtual void closeUIScenes(int iPad, bool forceIPad = false) = 0;

        virtual UIScene* getTopScene(int iPad, EUILayer layer = EUILayer::Scene, 
            EUIGroup group = EUIGroup::PAD) = 0;
        virtual bool isSceneInStack(int iPad, EUIScene scene) = 0;
        virtual UIScene* findScene(EUIScene sceneType) = 0;
    };

    __declspec(dllexport) IUIController* getUIController();
}