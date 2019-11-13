@echo off

if not exist ".\imgui" ( git submodule add -b docking https://github.com/ocornut/imgui.git )

if not exist ".\assimp" ( git submodule add https://github.com/Gurman8r/assimp.git )

if not exist ".\inih" ( git submodule add https://github.com/Gurman8r/inih.git )

if not exist ".\ImGuiColorTextEdit" ( git submodule add https://github.com/Gurman8r/ImGuiColorTextEdit.git )

if not exist ".\cpython" ( git submodule add https://github.com/python/cpython.git )

if not exist ".\stb" ( git submodule add https://github.com/nothings/stb.git )

if not exist ".\pybind11" ( git submodule add https://github.com/pybind/pybind11.git )

if not exist ".\glm" ( git submodule add https://github.com/g-truc/glm.git )

if not exist ".\gcem" ( git submodule add https://github.com/kthohr/gcem.git )

if not exist ".\openal-soft" ( git submodule add https://github.com/kcat/openal-soft.git )

if not exist ".\dirent" ( git submodule add https://github.com/tronkko/dirent.git )
	
if not exist ".\freetype2" ( git submodule add https://github.com/aseprite/freetype2.git )

if not exist ".\PDCurses" ( git submodule add https://github.com/wmcbrine/PDCurses.git )

if not exist ".\mega-sdk" ( git submodule add https://github.com/meganz/sdk.git mega-sdk )

if not exist ".\ImGuizmo" ( git submodule add https://github.com/CedricGuillemet/ImGuizmo.git )

if not exist ".\premake-core" ( git submodule add https://github.com/premake/premake-core.git )

if not exist ".\glew" ( git submodule add https://github.com/nigels-com/glew.git )

if not exist ".\lua" ( git submodule add https://github.com/lua/lua.git )

if not exist ".\RakNet" ( git submodule add https://github.com/facebookarchive/RakNet.git )

if %ERRORLEVEL% NEQ 0" ( pause )