#include "../include/input.hpp"
#include <windows.h>

namespace legacysdk::input {
    bool iskey(int vk) {
        return (GetAsyncKeyState(vk) & 0x8000) != 0;
    }

    int getkey() {
        for (int vk = 8; vk <= 255; vk++) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                return vk;
            }
        }
        return 0;
    }
}