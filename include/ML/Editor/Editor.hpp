#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Engine/Engine.hpp>
#include <ML/Editor/MainMenuBar.hpp>
#include <ML/Editor/Terminal.hpp>
#include <ML/Editor/Builder.hpp>
#include <ML/Editor/Browser.hpp>
#include <ML/Editor/ImGui_Builtin.hpp>
#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/TextEditor.hpp>
#include <ML/Editor/SceneView.hpp>
#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/Project.hpp>
#include <ML/Editor/NetworkHUD.hpp>
#include <ML/Editor/Profiler.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	//class Engine;
	//class EventSystem;
	//class Preferences;
	//class Resources;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Editor(Engine & engine);
		~Editor();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Browser		browser;
		Builder		builder;
		MainMenuBar	mainMenu;
		Dockspace	dockspace;
		Inspector	inspector;
		NetworkHUD	network;
		Profiler	profiler;
		Project		project;
		SceneView	sceneView;
		Terminal	terminal;
		TextEditor	textEditor;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool show_imgui_demo	= false;
		bool show_imgui_metrics	= false;
		bool show_imgui_style	= false;
		bool show_imgui_about	= false;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Engine		& engine()		const;
		EventSystem	& eventSystem() const;
		Preferences & prefs()		const;
		Resources	& resources()	const;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		Engine & m_engine;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_