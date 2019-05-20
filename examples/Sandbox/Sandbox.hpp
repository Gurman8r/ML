#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/EditorApplication.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Graphics/Text.hpp>
#include <ML/Physics/PhysicsWorld.hpp>

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

extern "C"
{
	ML_PLUGIN_API ml::Application * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Sandbox final : public ml::EditorApplication
	{
		enum Rigidbody_ID : int32_t
		{
			RB_BORG,
			RB_CUBE,
			RB_NAVBALL,
			RB_MOON,
			RB_EARTH,
			RB_GROUND,

			MAX_DEMO_RIGIDBODY
		};

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Sandbox(ml::EventSystem & eventSystem);
		~Sandbox();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEvent(const ml::IEvent * value) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEnter	(const ml::EnterEvent	& ev) override;
		void onLoad		(const ml::LoadEvent	& ev) override;
		void onStart	(const ml::StartEvent	& ev) override;
		void onUpdate	(const ml::UpdateEvent	& ev) override;
		void onDraw		(const ml::DrawEvent	& ev) override;
		void onGui		(const ml::GuiEvent		& ev) override;
		void onShutdown	(const ml::ShutdownEvent& ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		using TextTable = typename ml::HashMap<ml::String, ml::Text>;

		struct MyData final : public ml::INonCopyable
		{
			ml::SStream			rdstr		= ml::SStream();
			ml::StreamBuf *		rdbuf		= NULL;
			ml::String			res_data	= {};
			ml::String			res_path	= {};
			ml::String			manifest	= {};
			ml::VAO				vao			= {};
			ml::VBO				vbo			= {};
			TextTable			text		= {};
			ml::String			title		= {};
			ml::Entity *		light		= NULL;
			ml::Entity *		camera		= NULL;
			bool				cameraOrbit = true;
			float				cameraSpeed = 1.0f;
			int32_t				effectMode	= 3;
			ml::PhysicsWorld	physWorld	= {};

		} sandbox;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SANDBOX_HPP_