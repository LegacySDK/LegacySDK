# LegacySDK
LegacySDK is a modding SDK for Minecraft Legacy Edition (right now a glorified DLL injector)

# TO ANYONE READING MY SOURCE CODE
if you're a C++ developer, PLEASE FOR THE LOVE OF GOD DM ME ON DISCORD `misko.bin`. ESPECIALLY IF YOU KNOW `windows.h` WELL \
i need someone to help me create hooks (like XInput9_1_0.dll, DSOUND.dll or something) so people dont need to add this to their source code:
```cpp
char sdk[MAX_PATH];
strcpy_s(sdk, exeDir);
strcat_s(sdk, MAX_PATH, "\\legacysdk.dll");
	
HMODULE hSDK = LoadLibraryA(sdk);
	
if (!hSDK) {
  char msg[128];
  sprintf_s(msg, "fail\nGLE=%lu", GetLastError());
  MessageBoxA(NULL, msg, "LegacySDK", MB_OK | MB_ICONERROR);
}
```

i also REALLY REALLY need a designer (for icon, banner or whatever)

# compiling Minecraft Legacy
you first gotta get the source code of it somewhere, make sure it isnt the bad/fake ps3 source code, but the one thats `MinecraftConsoles` and has `Windows64` folders in `Minecraft.Client` (or similar), its password **MUST** be `fuckanon`

## 1. Prerequisites
Get Visual Studio 2012 (can be professional idc, as long it works) \
Import `MinecraftConsoles` onto Visual Studio 2012 (the project file) \
Find `Minecraft.Client` -> Right Click it -> Set as Startup

<img width="633" height="890" alt="image" src="https://github.com/user-attachments/assets/2c3db0d0-e618-46ff-81ef-5d6ad471081b" />

## 2. Opening `Windows64_Minecraft.cpp`
Yeah. you gotta do that to run the game normally. \
Open up `Minecraft.Client` -> find `Windows64` folder. \
After you've done that, open up the `Source files` folder. \
Inside of there you'll see `Windows64_Minecraft.cpp`

<img width="447" height="930" alt="image" src="https://github.com/user-attachments/assets/6a7c3fb8-6c34-473d-bd0c-a5056f2fe733" />

## 2.1 Modifying `Windows64_Minecraft.cpp` (D3D11 DEBUG FLAGS)
There are things you need to modify (such as createDeviceFlags) etc. \
Press `Ctrl + F` and then paste in ``createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;``
You should find it on Line ~480. Comment it out. Trust me. \
<img width="567" height="117" alt="image" src="https://github.com/user-attachments/assets/8e1232dd-e8e2-429b-988f-59047050e1c8" />

## 2.2. Modifying `Windows64_Minecraft.cpp` (_tWinMain stuff)
Add this:
```cpp
/* ... */
UNREFERENCED_PARAMETER(hPrevInstance);
UNREFERENCED_PARAMETER(lpCmdLine);

// append this to source code 

char exePath[MAX_PATH];
GetModuleFileNameA(NULL, exePath, MAX_PATH);
	
char exeDir[MAX_PATH];
strcpy_s(exeDir, exePath);
	
char* lastbs = strrchr(exeDir, '\\');
if (lastbs) *lastbs = '\0';
	
char gameDir[MAX_PATH];
strcpy_s(gameDir, exeDir);
	
char* x64_pos = strstr(gameDir, "\\x64\\");
if (x64_pos) {
  *x64_pos = 0;
  strcat_s(gameDir, MAX_PATH, "\\Minecraft.Client");
}
	
SetCurrentDirectoryA(gameDir);

// end
if (lpCmdLine) { /* ... */ }
```

## 3. Building
Right Click `Solution 'MinecraftConsoles' (2 projects)` -> `Build Solution` (or `Rebuild Solution`)

Now; make sure your settings are like this:

<img width="544" height="173" alt="image" src="https://github.com/user-attachments/assets/86b3a91c-04af-48c2-ae67-6c89ca0f5f1d" />

(MAKE SURE ITS AUTO, DEBUG, WINDOWS64) \
(you can also replace Debug with Release if you'd like)

Press `Local Windows Debugger` to start the game, you'll notice you cannot press anything unless you have a controller with DS4Windows or similar, since you gotta modify `Input.cpp` & `Input.hpp` files for mouse & keyboard

# Example
<img width="1624" height="870" alt="image" src="https://github.com/user-attachments/assets/91aeff1e-bf16-4158-b4eb-1d47786d473e" />
Code:

```cpp
#include <legacysdk/legacysdk.hpp>

using namespace legacysdk::log;

$execute([]() {
    info("mrbeast");
});
```

oh and minecraft legacy edition goes to 4J Studios & Mojang Studios

## Star History
<a href="https://www.star-history.com/?repos=LegacySDK%2FLegacySDK&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/chart?repos=LegacySDK/LegacySDK&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/chart?repos=LegacySDK/LegacySDK&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/chart?repos=LegacySDK/LegacySDK&type=date&legend=top-left" />
 </picture>
</a>
