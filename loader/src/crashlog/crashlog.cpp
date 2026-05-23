#include "crashlog.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <random>
#include <dbghelp.h>
#include <commctrl.h>

#undef min
#undef max

#pragma comment(lib, "dbghelp.lib")

namespace fs = std::filesystem;

namespace crashlog {
    static info g_info = {};
    static bool g_crashInit = false;

    std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "+0000";
        return oss.str();
    }

    const char* random() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(CRASH_MESSAGES) / sizeof(CRASH_MESSAGES[0]) - 1);
        return CRASH_MESSAGES[dis(gen)];
    }

    bool crashdir() {
        try {
            fs::path crashesDir = fs::current_path() / "crashes";
            if (!fs::exists(crashesDir)) {
                fs::create_directories(crashesDir);
            }
            return true;
        } catch (...) {
            return false;
        }
    }

    std::string formatReg(const reg& regs) {
        std::ostringstream oss;
        oss << "RAX: 0x" << std::hex << std::uppercase << regs.rax << std::dec << std::nouppercase << "\n";
        oss << "RBX: 0x" << std::hex << std::uppercase << regs.rbx << std::dec << std::nouppercase << "\n";
        oss << "RCX: 0x" << std::hex << std::uppercase << regs.rcx << std::dec << std::nouppercase << "\n";
        oss << "RDX: 0x" << std::hex << std::uppercase << regs.rdx << std::dec << std::nouppercase << "\n";
        oss << "RBP: 0x" << std::hex << std::uppercase << regs.rbp << std::dec << std::nouppercase << "\n";
        oss << "RSP: 0x" << std::hex << std::uppercase << regs.rsp << std::dec << std::nouppercase << "\n";
        oss << "RDI: 0x" << std::hex << std::uppercase << regs.rdi << std::dec << std::nouppercase << "\n";
        oss << "RSI: 0x" << std::hex << std::uppercase << regs.rsi << std::dec << std::nouppercase << "\n";
        oss << "RIP: 0x" << std::hex << std::uppercase << regs.rip << std::dec << std::nouppercase << "\n";
        oss << "R8: 0x"  << std::hex << std::uppercase << regs.r8 << std::dec << std::nouppercase  << "\n";
        oss << "R9: 0x"  << std::hex << std::uppercase << regs.r9 << std::dec << std::nouppercase  << "\n";
        oss << "R10: 0x" << std::hex << std::uppercase << regs.r10 << std::dec << std::nouppercase << "\n";
        oss << "R11: 0x" << std::hex << std::uppercase << regs.r11 << std::dec << std::nouppercase << "\n";
        oss << "R12: 0x" << std::hex << std::uppercase << regs.r12 << std::dec << std::nouppercase << "\n";
        oss << "R13: 0x" << std::hex << std::uppercase << regs.r13 << std::dec << std::nouppercase << "\n";
        oss << "R14: 0x" << std::hex << std::uppercase << regs.r14 << std::dec << std::nouppercase << "\n";
        oss << "R15: 0x" << std::hex << std::uppercase << regs.r15 << std::dec << std::nouppercase;
        return oss.str();
    }

    std::string formatSTrace(const std::vector<StackFrame>& trace) {
        std::ostringstream oss;
        for (const auto& frame : trace) {
            oss << "- " << frame.module;
            if (!frame.offset.empty()) {
                oss << " + " << frame.offset;
            }
            if (!frame.function.empty()) {
                oss << " (" << frame.function;
                if (!frame.file.empty()) {
                    oss << " | " << frame.file;
                    if (frame.line > 0) {
                        oss << ":" << frame.line;
                    }
                }
                oss << ")";
            }
            oss << "\n";
        }
        return oss.str();
    }

    std::string generate(const info& info) {
        std::ostringstream oss;
        oss << info.timestamp << "\n";
        oss << random() << "\n\n";
        oss << "== Loader Information ==\n";
        oss << "Loader Version: v" << info.loaderVersion << "\n";
        oss << "Platform: " << info.platform << "\n";
        oss << "Installed mods: " << info.installedModsCount << "\n";
        oss << "Failed to load: " << info.failedModsCount << "\n\n";
        oss << "== Exception Information ==\n";
        oss << info.exceptionType << ": " << info.exceptionMessage << "\n";
        oss << "Faulty Module: " << (info.faultyModuleName.empty() ? "<Unknown>" : info.faultyModuleName) << "\n";
        if (!info.faultyModFriendlyName.empty()) {
            oss << "Faulty Mod: " << info.faultyModFriendlyName << "\n";
        } else if (!info.faultyModPath.empty()) {
            oss << "Faulty Mod Path: " << info.faultyModPath << "\n";
        }
        oss << "Crashed thread: " << info.crashedThread << "\n\n";
        oss << "== Stack Trace ==\n";
        oss << formatSTrace(info.stackTrace) << "\n";
        oss << "== Register States ==\n";
        oss << formatReg(info.registers) << "\n";

        return oss.str();
    }

    bool write(const std::string& report) {
        try {
            if (!crashdir()) {
                return false;
            }

            fs::path crashesDir = fs::current_path() / "crashes";

            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::ostringstream filename;
            filename << "crash_" << std::put_time(std::gmtime(&time), "%Y%m%d_%H%M%S") << ".log";
            
            fs::path reportPath = crashesDir / filename.str();

            std::ofstream file(reportPath, std::ios::out | std::ios::trunc);
            if (!file.is_open()) {
                return false;
            }

            file << report;
            file.close();

            return true;
        } catch (...) {
            return false;
        }
    }

    void set(const info& info) {
        g_info = info;
    }

    void init_() {
        g_crashInit = true;
    }

    bool crashed() {
        try {
            fs::path crashesDir = fs::current_path() / "crashes";
            if (!fs::exists(crashesDir)) {
                return false;
            }

            for (const auto& entry : fs::directory_iterator(crashesDir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".log") {
                    auto lastWriteTime = fs::last_write_time(entry);
                    auto now = std::chrono::system_clock::now();
                    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        lastWriteTime - fs::file_time_type::clock::now() + now
                    );
                    auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(now - sctp).count();
                    
                    if (timeDiff < 3600) {
                        return true;
                    }
                }
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    std::string lastcrash() {
        try {
            fs::path crashesDir = fs::current_path() / "crashes";
            if (!fs::exists(crashesDir)) {
                return "";
            }

            fs::file_time_type lastTime = fs::file_time_type::min();
            fs::path lastPath;

            for (const auto& entry : fs::directory_iterator(crashesDir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".log") {
                    auto writeTime = fs::last_write_time(entry);
                    if (writeTime > lastTime) {
                        lastTime = writeTime;
                        lastPath = entry.path();
                    }
                }
            }

            return lastPath.empty() ? "" : lastPath.string();
        } catch (...) {
            return "";
        }
    }

    void triggerCrash(const char* message) {
        info crashInfo;
        crashInfo.timestamp = timestamp();
        crashInfo.exceptionType = "Exception FUCKYOU";
        crashInfo.exceptionMessage = message;
        crashInfo.faultyModuleName = "fucker.exe";
        crashInfo.faultyModFriendlyName = "FuckYou";
        crashInfo.crashedThread = "Main";
        crashInfo.loaderVersion = 1;
        crashInfo.platform = "Windows x64";
        crashInfo.installedModsCount = 0;
        crashInfo.failedModsCount = 0;

        StackFrame frame;
        frame.module = "crashlog.cpp";
        frame.function = "_FuckYou()";
        frame.offset = "0x4655434B";
        crashInfo.stackTrace.push_back(frame);

        std::string report = generate(crashInfo);
        write(report);
        MessageBoxA(
            NULL,
            report.c_str(),
            "LegacySDK - Crash Report",
            MB_OK | MB_ICONERROR
        );
        Sleep(500);
        // close game
        ExitProcess(1);
    }
}
