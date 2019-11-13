#!/bin/bash
git submodule sync

[ ! -d "./imgui" ] && git submodule add --force --force  -b docking https://github.com/Gurman8r/imgui.git

[ ! -d "./assimp" ] && git submodule add --force --force  https://github.com/Gurman8r/assimp.git

[ ! -d "./inih" ] && git submodule add --force --force  https://github.com/Gurman8r/inih.git

[ ! -d "./ImGuiColorTextEdit" ] && git submodule add --force --force  https://github.com/Gurman8r/ImGuiColorTextEdit.git

[ ! -d "./cpython" ] && git submodule add --force --force  https://github.com/python/cpython.git

[ ! -d "./stb" ] && git submodule add --force --force  https://github.com/nothings/stb.git

[ ! -d "./pybind11" ] && git submodule add --force --force  https://github.com/pybind/pybind11.git

[ ! -d "./glm" ] && git submodule add --force --force  https://github.com/g-truc/glm.git

[ ! -d "./gcem" ] && git submodule add --force --force  https://github.com/kthohr/gcem.git

[ ! -d "./openal-soft" ] && git submodule add --force --force  https://github.com/kcat/openal-soft.git

[ ! -d "./dirent" ] && git submodule add --force --force  https://github.com/tronkko/dirent.git
	
[ ! -d "./freetype2" ] && git submodule add --force --force  https://github.com/aseprite/freetype2.git

[ ! -d "./PDCurses" ] && git submodule add --force --force  https://github.com/wmcbrine/PDCurses.git

[ ! -d "./mega-sdk" ] && git submodule add --force --force  https://github.com/meganz/sdk.git mega-sdk

[ ! -d "./ImGuizmo" ] && git submodule add --force --force  https://github.com/CedricGuillemet/ImGuizmo.git

[ ! -d "./premake-core" ] && git submodule add --force --force  https://github.com/premake/premake-core.git

[ ! -d "./glew" ] && git submodule add --force --force  https://github.com/nigels-com/glew.git

[ ! -d "./lua" ] && git submodule add --force --force  https://github.com/lua/lua.git

[ ! -d "./RakNet" ] && git submodule add --force --force  https://github.com/facebookarchive/RakNet.git

git submodule update --init --recursive
git pull
exit 0