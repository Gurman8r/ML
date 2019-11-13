@echo off
git submodule sync

if not exist "..\ext\imgui" ( git submodule add --force -b docking https://github.com/Gurman8r/imgui.git )

if not exist "..\ext\assimp" ( git submodule add --force https://github.com/Gurman8r/assimp.git )

if not exist "..\ext\inih" ( git submodule add --force https://github.com/Gurman8r/inih.git )

if not exist "..\ext\ImGuiColorTextEdit" ( git submodule add --force https://github.com/Gurman8r/ImGuiColorTextEdit.git )

if not exist "..\ext\cpython" ( git submodule add --force https://github.com/python/cpython.git )

if not exist "..\ext\stb" ( git submodule add --force https://github.com/nothings/stb.git )

if not exist "..\ext\pybind11" ( git submodule add --force https://github.com/pybind/pybind11.git )

if not exist "..\ext\glm" ( git submodule add --force https://github.com/g-truc/glm.git )

if not exist "..\ext\gcem" ( git submodule add --force https://github.com/kthohr/gcem.git )

if not exist "..\ext\openal-soft" ( git submodule add --force https://github.com/kcat/openal-soft.git )

if not exist "..\ext\dirent" ( git submodule add --force https://github.com/tronkko/dirent.git )
	
if not exist "..\ext\freetype2" ( git submodule add --force https://github.com/aseprite/freetype2.git )

if not exist "..\ext\PDCurses" ( git submodule add --force https://github.com/wmcbrine/PDCurses.git )

if not exist "..\ext\mega-sdk" ( git submodule add --force https://github.com/meganz/sdk.git mega-sdk )

if not exist "..\ext\ImGuizmo" ( git submodule add --force https://github.com/CedricGuillemet/ImGuizmo.git )

if not exist "..\ext\premake-core" ( git submodule add --force https://github.com/premake/premake-core.git )

if not exist "..\ext\glew" ( git submodule add --force https://github.com/nigels-com/glew.git )

if not exist "..\ext\glfw" ( git submodule add --force https://github.com/Gurman8r/glfw.git )

if not exist "..\ext\lua" ( git submodule add --force https://github.com/lua/lua.git )

if not exist "..\ext\RakNet" ( git submodule add --force https://github.com/facebookarchive/RakNet.git )

git submodule update --init --recursive
git pull
if %ERRORLEVEL% NEQ 0" ( pause )