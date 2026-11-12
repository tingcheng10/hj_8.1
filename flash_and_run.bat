@echo off
setlocal
chcp 65001 >nul

set "PROJ=D:\cubemax\7.1"
set "CMAKE=D:\CLion 2026.2.4\bin\cmake\win\x64\bin\cmake.exe"
set "OC=D:\OpenOCD-20260302-0.12.0\bin\openocd.exe"
set "OCS=D:\OpenOCD-20260302-0.12.0\share\openocd\scripts"
set "ELF=D:/cubemax/7.1/build/Debug/7.1.elf.elf"

echo ============================================
echo   7.1  build + flash + run
echo ============================================
echo.
echo [1/2] Building ...
"%CMAKE%" --build "%PROJ%\build\Debug"
if errorlevel 1 (
  echo.
  echo BUILD FAILED - fix the errors in CLion, then run this again.
  echo.
  pause
  exit /b 1
)

echo.
echo [2/2] Flashing ...
"%OC%" -s "%OCS%" -f "interface/stlink.cfg" -f "target/stm32f4x.cfg" ^
  -c "adapter speed 1000" -c "reset_config none" ^
  -c "init" -c "halt" ^
  -c "program %ELF% verify" ^
  -c "reset run" -c "shutdown"

if errorlevel 1 (
  echo.
  echo FLASH FAILED.
  echo   1) Re-plug the STLINK Type-C port of the C board (ST-Link locks up after some errors).
  echo   2) Close any CLion debug session / other OpenOCD / STM32CubeProgrammer.
  echo   3) Make sure the board is powered on.
  echo.
  pause
  exit /b 1
)

echo.
echo DONE - the board is running the new firmware.
timeout /t 2 >nul
exit /b 0
