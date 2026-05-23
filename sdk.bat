@echo off
setlocal enabledelayedexpansion

set ROOT=%~dp0
set SDK_SRC=%ROOT%sdk\include
set OUT=%ROOT\sdk_dist



if exist "%OUT%" rmdir /s /q "%OUT%"
mkdir "%OUT%"
mkdir "%OUT%\legacysdk"

copy /Y "%SDK_SRC%\log.hpp" "%OUT%\legacysdk\log.hpp"
copy /Y "%SDK_SRC%\input.hpp" "%OUT%\legacysdk\input.hpp"
copy /Y "%SDK_SRC%\mod.hpp" "%OUT%\legacysdk\mod.hpp"

(
echo #pragma once
echo.
echo #include "log.hpp"
echo #include "input.hpp"
echo #include "mod.hpp"
echo.
echo namespace legacysdk {
echo     using namespace log;
echo     using namespace input;
echo     using namespace mod;
echo }
) > "%OUT%\legacysdk\legacysdk.hpp"

echo don
echo at %OUT%

pause