#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Editor/MainMenuBar.hpp>
#include <ML/Editor/Terminal.hpp>
#include <ML/Editor/Builder.hpp>
#include <ML/Editor/Browser.hpp>
#include <ML/Editor/ImGui_Builtin.hpp>
#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/TextEditor.hpp>
#include <ML/Editor/SceneView.hpp>
#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/ResourceView.hpp>
#include <ML/Editor/NetworkHUD.hpp>
#include <ML/Editor/Profiler.hpp>

#define ML_Editor ml::Editor::getInstance()

namespace ml
{
	class ML_EDITOR_API Editor final
		: public ITrackable
		, public ISingleton<Editor>
	{
		friend class ISingleton<Editor>;

	private:
		Editor();
		~Editor();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_mainMenuBar	= true;
		bool show_dockspace		= true;
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_imgui_demo	= false;
		bool show_imgui_metrics	= false;
		bool show_imgui_style	= false;
		bool show_imgui_about	= false;
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_terminal		= true;
		bool show_browser		= true;
		bool show_builder		= true;
		bool show_inspector		= false;
		bool show_sceneView		= true;
		bool show_textEditor	= false;
		bool show_resourceView	= true;
		bool show_profiler		= true;
		bool show_network		= false;
		/* * * * * * * * * * * * * * * * * * * * */
	};

	
}

#endif // !_ML_EDITOR_HPP_