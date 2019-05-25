#ifndef _SANDBOX_HPP_
#define _SANDBOX_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Text.hpp>
#include <ML/Engine/PhysicsWorld.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_PLUGIN_API Sandbox final : public ml::EditorPlugin
	{
	public:
		Sandbox(ml::EventSystem & eventSystem);
		~Sandbox();

	public:
		void onEvent(const ml::Event * value) override;

	private:
		void onEnter	(const ml::EnterEvent	& ev) override;
		void onStart	(const ml::StartEvent	& ev) override;
		void onUpdate	(const ml::UpdateEvent	& ev) override;
		void onDraw		(const ml::DrawEvent	& ev) override;
		void onGui		(const ml::GuiEvent		& ev) override;
		void onExit		(const ml::ExitEvent	& ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */

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

		using TextTable = typename ml::HashMap<ml::String, ml::Text>;

		struct MyData final : public ml::INonCopyable
		{
			ml::VAO				vao			= {};
			ml::VBO				vbo			= {};
			TextTable			text		= {};
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