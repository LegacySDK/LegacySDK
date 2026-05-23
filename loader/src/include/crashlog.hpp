#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include <ctime>

namespace crashlog {
    inline constexpr const char* CRASH_MESSAGES[] = {
        "Why?",
        "Whoopsies! An unhandled exception has occurred.",
        "The fast path is not available because one of the required library is not installed. Falling back to torch implementation.",
        "It ded",
        "Something happened. (it's probably bad)",
        "August 12 2036, the heat death of the universe.",
        "ValueError: --load_best_model_at_end requires the saving steps to be a round multiple of the evaluation steps, but found 265, which is not a round multiple of 159.",
        "Warning: You are sending unauthenticated requests to the HF Hub. Please set a HF_TOKEN to enable higher rate limits and faster downloads.",
        "Yes, you're paying to expand jenson's spatula collection.",
        "So thats where the layoffs cash money went in tech.",
        "you mean CUDA?",
        "The cake is a lie.",
        "The FitnessGram™ Pacer Test is a multistage aerobic capacity test that progressively gets more difficult as it continues.",
        "Nyoooo :c I'm so sowwy >~< I did the cwashieeeee!",
        "fortnite balls",
        "d",
        "My name is Markiplier",
        "no tata znow nie dziala te wideo :(",
        "OwO what's this? An error message? How cute!",
        "UwU OOPSIE WOOPSIE!! Uwu We made a fucky wucky mistake!! OwO",
        "I-I'm sowwy, I didn't mean to cwash the game! >~<",
       /*
        "Everything's going to plan. No, really, that was supposed to happen.",
        "Uh... Did I do that?",
        "Oops.",
        "Why did you do that?",
        "I feel sad now :(",
        "My bad.",
        "Time to uninstall some mods!",
        "I let you down. Sorry :(",
        "On the bright side, I bought you a teddy bear!",
        "Your Mega Hack crashed! (Just kidding)",
        "Oh - I know what I did wrong!",
        "Hey, that tickles! Hehehe!",
        "I blame 4J Studios",
        "Don't be sad. I'll do better next time, I promise!",
        "Don't be sad, have a hug! <3",
        "I just don't know what went wrong :(",
        "Shall we play a game?",
        "Quite honestly, I wouldn't worry myself about that.",
        "Sorry :(",
        "Surprise! Haha. Well, this is awkward.",
        "Would you like a cupcake?",
        "This doesn't make any sense!",
        "Why is it breaking :(",
        "Don't do that.",
        "Ouch. That hurt :(",
        "You're mean.",
        "But it works on my machine.",
        "Hi Absolute!",
        "The chicken is on fire!",
        "I've accidentally crashed the game! /RubRub",
        "It's a feature, not a bug!",
        "Try turning it off and on again.",
        "Boo!",
        "Whoopsies! An unhandled exception has occurred.",
        "Geometry Dash isn't Geometry Dashing anymore",
        "$ sudo rm -rf /",
        "To the mod developer: Please 309 yourself",
        "Let's pretend this didn't happen.",
        "Skill issue",
        "Out of cheese error.",
        "Error 404: Fun not found.",
        "A wild bug appeared!",
        "Congratulations! You found a bug.",
        "Don't panic! It's just a minor setback.",
        "Cosmic rays caused a bit flip, sorry!",
        "I'm not crying, you're crying.",
        "Insert witty error message here.",
        "Mission failed successfully.",
        "Game over. Insert coin to continue.",
        "You shall not pass! Oh wait, you did...",
        "The cake is a lie!",
        "Quick, look over there! *runs away*",
        "To be continued...",
        "In case of fire, use water.",
        "Did you try blowing on the cartridge?",
        "You found an easter egg! Just kidding, it's a crash.",
        "Have you tried yelling at the screen?",
       */
    };

    struct reg {
        DWORD64 rax;
        DWORD64 rbx;
        DWORD64 rcx;
        DWORD64 rdx;
        DWORD64 rbp;
        DWORD64 rsp;
        DWORD64 rdi;
        DWORD64 rsi;
        DWORD64 rip;
        DWORD64 r8;
        DWORD64 r9;
        DWORD64 r10;
        DWORD64 r11;
        DWORD64 r12;
        DWORD64 r13;
        DWORD64 r14;
        DWORD64 r15;
    };

    struct StackFrame {
        std::string module;
        std::string function;
        std::string offset;
        std::string file;
        int line;
    };

    struct info {
        std::string timestamp;
        std::string exceptionType;
        std::string exceptionMessage;
        std::string faultyModuleName;
        std::string faultyModPath;
        std::string faultyModFriendlyName;
        std::string crashedThread;
        reg registers;
        std::vector<StackFrame> stackTrace;
        int loaderVersion;
        std::string platform;
        int installedModsCount;
        int failedModsCount;
    };

    void init_();
    std::string generate(const info& info);
    bool write(const std::string& report);
    const char* random();
    std::string formatReg(const reg& regs);
    std::string formatSTrace(const std::vector<StackFrame>& trace);
    bool crashdir();
    std::string timestamp();
    void set(const info& info);
    bool hasCrashed();
    std::string lastcrash();
    void triggerCrash(const char* message = "fuck");
}
