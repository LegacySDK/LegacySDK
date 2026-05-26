@echo off
:: made by miskaa
echo start

:::::::::::::::::::::::::::::::::::::::::

rd /s .\build
mkdir build && cd build
cmake ..

timeout /t 1 > nul

cmake --build . --config Release

:::::::::::::::::::::::::::::::::::::::::

cd ..
cd launcher
rd /s /q .\build
mkdir build && cd build
cmake ..

timeout /t 1 > nul

cmake --build . --config Release

:::::::::::::::::::::::::::::::::::::::::

cd ..\..

:::::::::::::::::::::::::::::::::::::::::

set XINPUT=launcher\build\Release\XINPUT9_1_0.dll
set LEGACYSDK=build\Release\legacysdk.dll

set MINECRAFT=..\..\x64\Debug\

del "%MINECRAFT%XINPUT9_1_0.dll" /q
del "%MINECRAFT%legacysdk.dll" /q

copy /Y "%XINPUT%" "%MINECRAFT%"
copy /Y "%LEGACYSDK%" "%MINECRAFT%"

echo done