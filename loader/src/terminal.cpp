#include "terminal.hpp"
#include <windows.h>
#include <cstdio>
#include <cstdarg>

static bool g_init = false;

void terminit() {
    if (g_init) return;
    g_init = true;

    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$",  "r", stdin);

    SetConsoleTitleA("legacysdk debug terminal");
}

void termprint(const char* txt) {
    if (!g_init) terminit();
    printf("%s\n", txt);
}

void termprintf(const char* fmt, ...) {
    if (!g_init) terminit();

    char buf[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, args);
    va_end(args);

    printf("%s\n", buf);
}