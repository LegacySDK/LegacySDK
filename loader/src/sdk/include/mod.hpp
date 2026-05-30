#pragma once
#include <string>
#include <functional>

namespace legacysdk::mod {

enum class me {
    Init,
    Load,
    Enable,
    Disable,
    Unload,
    Shutdown
};

using eh = std::function<void()>;
struct info {
    std::string id;
    std::string name;
    std::string version;
    std::string description;
    std::string author;
};

class IMod {
public:
    virtual ~IMod() = default;
    virtual void onInit() {}
    virtual void onLoad() {}
    virtual void onEnable() {}
    virtual void onDisable() {}
    virtual void onUnload() {}
    virtual void onShutdown() {}
    
    virtual info getInfo() = 0;
};

extern "C" {
    __declspec(dllexport) IMod* createMod();
    __declspec(dllexport) void destroyMod(IMod* mod);
}

#define $on_init(func) \
    namespace { \
        struct InitReg { \
            InitReg() { legacysdk::mod::reg(legacysdk::mod::me::Init, func); } \
        } g_initReg; \
    }

#define $on_load(func) \
    namespace { \
        struct LoadReg { \
        LoadReg() { legacysdk::mod::reg(legacysdk::mod::me::Load, func); } \
        } g_loadReg; \
    }

#define $on_enable(func)    \
    namespace {             \
        struct EnableReg {  \
        EnableReg() { legacysdk::mod::reg(legacysdk::mod::me::Enable, func); } \
        } g_enableReg;      \
    }

#define $on_disable(func) \
    namespace { \
        struct DisableReg { \
        DisableReg() { legacysdk::mod::reg(legacysdk::mod::me::Disable, func); } \
        } g_disableReg; \
    }

#define $execute(func) $on_enable(func)

#define $on_ui_ready(func) $on_enable(func)

void reg(me event, eh handler);
void dispatchEvent(me event);

}