#ifndef _ML_EDITOR_HPP_
#define _ML_EDITOR_HPP_

#include <ML/Graphics/RenderWindow.hpp>
#include <ML/Editor/TerminalGui.hpp>
#include <ML/Editor/BuilderGui.hpp>
#include <ML/Editor/BrowserGui.hpp>
#include <ML/Editor/DockspaceGui.hpp>
#include <ML/Editor/SceneGui.hpp>
#include <ML/Editor/InspectorGui.hpp>
#include <ML/Editor/ResourceGui.hpp>
#include <ML/Editor/ProfilerGui.hpp>
#include <ML/Engine/EngineEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor final
		: public IObject
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
		DockspaceGui	dockspace;
		BrowserGui		browser;
		BuilderGui		builder;
		InspectorGui	inspector;
		ProfilerGui		profiler;
		ResourceGui		resources;
		SceneGui		scene;
		TerminalGui		terminal;

	private:
		std::streambuf * m_coutBuf; // cout redirect buffer
		SStream m_coutStr; // cout redirect stream
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_HPP_