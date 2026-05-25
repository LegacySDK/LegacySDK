#pragma once

namespace legacysdk::ui {
    class UIScene;

    typedef void(*FuncNavigateToHomeMenu)(void*);
    typedef bool(*FuncNavigateToScene)(void*, int, int, void*, int, int);
    typedef bool(*FuncNavigateBack)(void*, int, bool, int, int);
    typedef void(*FuncCloseAllPlayersScenes)(void*);
    typedef void(*FuncCloseUIScenes)(void*, int, bool);
    typedef UIScene*(*FuncGetTopScene)(void*, int, int, int);
    typedef bool(*FuncIsSceneInStack)(void*, int, int);
    typedef UIScene*(*FuncFindScene)(void*, int);

    __declspec(dllexport) void initUICONTROLLER(
        void* controller,
        FuncNavigateToHomeMenu pNavToHome,
        FuncNavigateToScene pNavToScene,
        FuncNavigateBack pNavBack,
        FuncCloseAllPlayersScenes pCloseAll,
        FuncCloseUIScenes pCloseUI,
        FuncGetTopScene pGetTop,
        FuncIsSceneInStack pIsInStack,
        FuncFindScene pFindScene
    );
}
