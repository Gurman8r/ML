#!/bin/bash
cd ../ext
git submodule sync

git submodule add --force -b docking https://github.com/Gurman8r/imgui.git
git submodule add --force https://github.com/Gurman8r/assimp.git
git submodule add --force https://github.com/Gurman8r/inih.git
git submodule add --force https://github.com/Gurman8r/ImGuiColorTextEdit.git
git submodule add --force https://github.com/python/cpython.git
git submodule add --force https://github.com/nothings/stb.git
git submodule add --force https://github.com/pybind/pybind11.git
git submodule add --force https://github.com/g-truc/glm.git
git submodule add --force https://github.com/kthohr/gcem.git
git submodule add --force https://github.com/kcat/openal-soft.git
git submodule add --force https://github.com/tronkko/dirent.git
git submodule add --force https://github.com/aseprite/freetype2.git
git submodule add --force https://github.com/wmcbrine/PDCurses.git
git submodule add --force https://github.com/meganz/sdk.git mega-sdk
git submodule add --force https://github.com/CedricGuillemet/ImGuizmo.git
git submodule add --force https://github.com/premake/premake-core.git
git submodule add --force https://github.com/nigels-com/glew.git
git submodule add --force https://github.com/Gurman8r/glfw.git
git submodule add --force https://github.com/lua/lua.git
git submodule add --force https://github.com/facebookarchive/RakNet.git

git submodule update --init --recursive
git pull
exit 0