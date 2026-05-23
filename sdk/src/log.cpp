#include "../include/log.hpp"
#include "terminal.hpp"

namespace legacysdk::log {

    void info(const char* msg) {
        termprintf("[info] %s", msg);
    }

    void warn(const char* msg) {
        termprintf("[warn] %s", msg);
    }

}