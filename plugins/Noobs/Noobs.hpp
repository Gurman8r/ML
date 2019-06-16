#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Surface.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PLUGIN_API Noobs final : public ml::EditorPlugin
	{
	public:
		explicit Noobs(ml::EventSystem & eventSystem);
		~Noobs();

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
		struct NoobsData : public ml::INonCopyable
		{
			ml::Surface * surf_main;
			ml::Surface * surf_post;
			ml::Entity	* ent_main;
			
			ml::vec2i res = { 1920, 1080 };
			ml::vec4f clearColor = ml::Color::Gray;

			bool showScene		= true;
			bool showBuilder	= true;

		} noobs;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_NOOBS_HPP_