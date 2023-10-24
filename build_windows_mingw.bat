@echo off
cmake -G "MinGW Makefiles" -S ./ -B build
cmake --build build --target=install --config=Release
pause
