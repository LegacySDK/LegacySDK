#include "../include/mod.hpp"
#include <map>
#include <vector>

namespace legacysdk::mod {
    static std::map<me, std::vector<eh>> g_eventHandlers;
    void reg(me event, eh handler) {
        g_eventHandlers[event].push_back(handler);
    }

    void dispatchEvent(me event) {
        auto it = g_eventHandlers.find(event);
        if (it != g_eventHandlers.end()) {
            for (auto& handler : it->second) {
                handler();
            }
        }
    }
}