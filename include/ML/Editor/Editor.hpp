#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Editor/Terminal.hpp>
#include <ML/Editor/Builder.hpp>
#include <ML/Editor/Browser.hpp>
#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/SceneView.hpp>
#include <ML/Editor/Inspector.hpp>
#include <ML/Editor/Project.hpp>
#include <ML/Editor/Profiler.hpp>
#include <ML/Engine/EngineEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public ITrackable
		, public EventListener
		, public INonCopyable
	{
	public:
		explicit Editor(EventSystem & eventSystem);
		~Editor();

	public:
		void onEvent(const Event * value) override;

	private:
		void onEnter	(const EnterEvent & ev);
		void onBeginGui	(const BeginGuiEvent & ev);
		void onGui		(const GuiEvent & ev);
		void onEndGui	(const EndGuiEvent & ev);
		void onExit		(const ExitEvent & ev);

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
		std::streambuf * m_coutBuf; // cout redirect buffer
		SStream m_coutStr; // cout redirect stream
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_