#pragma once

namespace legacysdk::ui {
    enum class EUIScene {
        MainMenu = 0,
        PauseMenu,
        ContainerMenu,
        InventoryMenu,
        CraftingMenu,
        EnchantingMenu,
        AnvilMenu,
        BeaconMenu,
        BrewingMenu,
        FurnaceMenu,
        HopperMenu,
        DispenserMenu,
        DropperMenu,
        BrewingStandMenu,
        HorseMenu,
        VillagerMenu,
        ShulkerBoxMenu,
        GameMenu,
        SettingsMenu,
        SkinPicker,
        StorageMenu,
        ProfileManager,
        ServerList,
        WorldSelection,
        CreateWorld,
        LevelEdit,
        DebugConsole,
        MarketingGuide,
        COUNT
    };

    enum class EUINavigationAction {
        NavigateToHomeMenu,
        NavigateToScene,
        NavigateBack,
        CloseAllPlayersScenes,
        CloseUIScenes
    };

    enum class EUILayer {
        Scene = 0,
        Overlay,
        Modal,
        Dialog,
        Toast,
        Debug,
        COUNT
    };

    enum class EUIGroup {
        PAD = 0,
        Standard,
        Effects,
        Background,
        Tooltips,
        Debug,
        COUNT
    };
}