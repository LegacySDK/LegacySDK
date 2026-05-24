# Installing the SDK to \\x64\\Debug\\ (or Release)

ok so
```cmd
REM make sure yo're in the LegacySDK-main or LegacySDK directory

REM LAUNCH THE GODDAMN Developer Command Prompt for VS 2022 TO COMPILE (or VS 2026, that later)

cd XInput
mkdir build && cd build
cmake .. && cmake --build . --config Release

cd ../..
REM should just be at LegacySDK-main or LegacySDK root folder

cd loader
mkdir build && cd build
cmake .. && cmake --build . --config Release
```

and then copy the files over to \\x64\\Debug\\ (or Release), files should look like this:
```
(other files like Minecraft.Client.exe etc.)

XINPUT9_1_0.dll
XINPUT9_1_0.lib
legacysdk.dll
legacysdk.lib
```
should work? idk, im gonna add releases and .github/workflows later