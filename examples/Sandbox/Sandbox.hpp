#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/EditorApplication.hpp>
#include <ML/Graphics/Text.hpp>
#include <ML/Graphics/Canvas.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "Audio"		"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Core"		"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Editor"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Engine"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Graphics"	"_" ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Network"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Physics"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
#	pragma comment(lib, "Window"	"_"	ML_CONFIGURATION "_" ML_PLATFORM_TARGET ".lib")
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

		struct MyData final
		{
			ml::SStream		rdstr;
			ml::StreamBuf * rdbuf = NULL;
			ml::Canvas		canvas;
			TextTable		text;
			ml::String		title;
			bool			isClient;
			bool			isServer;
			bool			cameraOrbit = true;
			float			cameraSpeed = 1.0f;
			int32_t			effectMode = 3;

		} self;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_