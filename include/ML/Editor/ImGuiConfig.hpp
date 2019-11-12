#ifndef _ML_IMGUI_CONFIG_HPP_
#define _ML_IMGUI_CONFIG_HPP_

#include <ML/Editor/Export.hpp>

#define IMGUI_API ML_EDITOR_API

# if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#	pragma comment(lib, "legacy_stdio_definitions")
# endif

extern "C" IMGUI_API int imgui_main(int, char **);

#endif // !_ML_IMGUI_CONFIG_HPP_