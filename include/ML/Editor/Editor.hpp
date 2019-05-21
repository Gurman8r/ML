#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Core/IEventListener.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/Terminal.hpp>
#include <ML/Editor/Builder.hpp>
#include <ML/Editor/Browser.hpp>
#include <ML/Editor/ImGui_Builtin.hpp>
#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/SceneView.hpp>
#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/Project.hpp>
#include <ML/Editor/Profiler.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public ITrackable
		, public IEventListener
		, public INonCopyable
	{
	public:
		explicit Editor(EventSystem & eventSystem);
		~Editor();

	public:
		void onEvent(const Event * value) override;

	private:
		void onGui(const GuiEvent & ev);

	public:
		Dockspace	dockspace;
		Browser		browser;
		Builder		builder;
		Inspector	inspector;
		Profiler	profiler;
		Project		project;
		SceneView	sceneView;
		Terminal	terminal;

	private:
		StreamBuf * m_coutBuf; // cout redirect buffer
		SStream		m_coutStr; // cout redirect stream
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_