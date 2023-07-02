@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
pushd .\build
cl -Zi ..\Main.cpp user32.lib gdi32.lib
.\Main.exe
popd