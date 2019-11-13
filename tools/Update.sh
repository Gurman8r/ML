#!/bin/bash
git submodule sync

[ ! -d "../ext/imgui" ] && git submodule add --force -b docking https://github.com/Gurman8r/imgui.git

[ ! -d "../ext/assimp" ] && git submodule add --force https://github.com/Gurman8r/assimp.git

[ ! -d "../ext/inih" ] && git submodule add --force https://github.com/Gurman8r/inih.git

[ ! -d "../ext/ImGuiColorTextEdit" ] && git submodule add --force https://github.com/Gurman8r/ImGuiColorTextEdit.git

[ ! -d "../ext/cpython" ] && git submodule add --force https://github.com/python/cpython.git

[ ! -d "../ext/stb" ] && git submodule add --force https://github.com/nothings/stb.git

[ ! -d "../ext/pybind11" ] && git submodule add --force https://github.com/pybind/pybind11.git

[ ! -d "../ext/glm" ] && git submodule add --force https://github.com/g-truc/glm.git

[ ! -d "../ext/gcem" ] && git submodule add --force https://github.com/kthohr/gcem.git

[ ! -d "../ext/openal-soft" ] && git submodule add --force https://github.com/kcat/openal-soft.git

[ ! -d "../ext/dirent" ] && git submodule add --force https://github.com/tronkko/dirent.git
	
[ ! -d "../ext/freetype2" ] && git submodule add --force https://github.com/aseprite/freetype2.git

[ ! -d "../ext/PDCurses" ] && git submodule add --force https://github.com/wmcbrine/PDCurses.git

[ ! -d "../ext/mega-sdk" ] && git submodule add --force https://github.com/meganz/sdk.git mega-sdk

[ ! -d "../ext/ImGuizmo" ] && git submodule add --force https://github.com/CedricGuillemet/ImGuizmo.git

[ ! -d "../ext/premake-core" ] && git submodule add --force https://github.com/premake/premake-core.git

[ ! -d "../ext/glew" ] && git submodule add --force https://github.com/nigels-com/glew.git

[ ! -d "../ext/glew" ] && git submodule add --force https://github.com/Gurman8r/glfw.git

[ ! -d "../ext/lua" ] && git submodule add --force https://github.com/lua/lua.git

[ ! -d "../ext/RakNet" ] && git submodule add --force https://github.com/facebookarchive/RakNet.git

git submodule update --init --recursive
git pull
exit 0