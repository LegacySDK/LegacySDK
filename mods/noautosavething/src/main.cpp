#include <legacysdk/legacysdk.hpp>
#include <windows.h>

using namespace legacysdk::log;

$execute([]() {
    info("noautosave mod loaded");

    Sleep(10000);

    info("sleep gone thing idk");

    Sleep(5000);

    info("sleep gone thing idk v2");
    legacysdk::ui::getUIController()
    ->navigateToHomeMenu();
    info("i ran");
});