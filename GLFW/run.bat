@echo off
mkdir build
copy ".\lib\*.dll"  ".\build\"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
pushd .\build
cl -EHsc -Zi ..\Main.cpp -I ..\include opengl32.lib user32.lib gdi32.lib shell32.lib kernel32.lib -MD -link ..\lib\*.lib
.\Main.exe
popd