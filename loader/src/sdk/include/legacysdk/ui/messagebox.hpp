#pragma once
#include <cstdint>

namespace legacysdk::ui {
    struct mbox {
        unsigned int uiTitle;
        unsigned int uiText;
        unsigned int* uiOptionA;
        unsigned int uiOptionC;
        unsigned long dwPad;
        int (*func)(void*, int, int);
        void* lpParam;
        wchar_t* pwchFormatString;
        unsigned long dwFocusButton;
    };
}