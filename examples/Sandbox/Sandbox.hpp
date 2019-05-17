#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/EditorApplication.hpp>
#include <ML/Graphics/Text.hpp>
#include <ML/Graphics/Canvas.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "Audio_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Core_"		ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Editor_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Engine_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Graphics_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Network_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Physics_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Window_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Sandbox final
		: public ml::EditorApplication
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Sandbox();
		~Sandbox();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEvent(const ml::IEvent * value) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEnter	(const ml::EnterEvent	* ev) override;
		void onLoad		(const ml::LoadEvent	* ev) override;
		void onStart	(const ml::StartEvent	* ev) override;
		void onUpdate	(const ml::UpdateEvent	* ev) override;
		void onDraw		(const ml::DrawEvent	* ev) override;
		void onGui		(const ml::GuiEvent		* ev) override;
		void onUnload	(const ml::UnloadEvent	* ev) override;
		void onExit		(const ml::ExitEvent	* ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		using TextTable = typename ml::HashMap<ml::String, ml::Text>;

		ml::SStream		m_rdstr;
		ml::StreamBuf * m_rdbuf = NULL;
		ml::Canvas		m_canvas;
		TextTable		m_text;
		ml::String		m_title;
		bool			m_isClient;
		bool			m_isServer;

		struct Globals
		{
			bool	camAuto		= false;
			float	camSpd		= 1.0f;
			int32_t	effectMode	= 3;
		
		} globals;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_