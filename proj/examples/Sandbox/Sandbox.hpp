#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/EditorApplication.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Graphics/Text.hpp>
#include <ML/Graphics/Canvas.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# if defined(ML_SYSTEM_WINDOWS)
#	pragma comment(lib, ML_LIB_STR("Audio")		)
#	pragma comment(lib, ML_LIB_STR("Core")		)
#	pragma comment(lib, ML_LIB_STR("Editor")	)
#	pragma comment(lib, ML_LIB_STR("Engine")	)
#	pragma comment(lib, ML_LIB_STR("Graphics")	)
#	pragma comment(lib, ML_LIB_STR("Network")	)
#	pragma comment(lib, ML_LIB_STR("Physics")	)
#	pragma comment(lib, ML_LIB_STR("Window")	)
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
		Sandbox(ml::EventSystem & eventSystem);
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

		struct MyData final : public ml::INonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			ml::SStream		rdstr		= ml::SStream();
			ml::StreamBuf * rdbuf		= NULL;
			ml::VAO			vao			= {};
			ml::VBO			vbo			= {};
			TextTable		text		= {};
			ml::String		title		= {};
			bool			isClient	= false;
			bool			isServer	= false;
			ml::Entity *	light		= NULL;
			ml::Entity *	camera		= NULL;
			bool			cameraOrbit = true;
			float			cameraSpeed = 1.0f;
			int32_t			effectMode	= 3;

			/* * * * * * * * * * * * * * * * * * * * */

			MyData(Sandbox & self) : m_self(self) {}

			inline Sandbox * operator->() { return &m_self; }
			inline Sandbox & operator *() { return  m_self; }

			/* * * * * * * * * * * * * * * * * * * * */

		private:
			Sandbox & m_self;

		} sandbox;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_