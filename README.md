# LegacySDK
LegacySDK is a modding SDK for Minecraft Legacy Edition (right now a glorified DLL injector)

# note
i REALLY REALLY need a designer (for icon, banner or whatever), thanks

# Compiling and running Minecraft Legacy Edition for Windows
Go over to <a href="./docs/COMPILE.md">COMPILE.md</a> to see how to compile Legacy Edition for Windows

# Getting the SDK
<a href="./docs/SDK.md">Click me and do all the steps here!</a>
<h5>though the game WILL CRASH on some versions</h5>

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

<h6>little note: XInput hooking or whatever was borrowed from Geode SDK, thank you :3</h6>
