#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Core/Transform.hpp>
#include <ML/Graphics/Color.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	class Entity;
	class Material;
	class Renderer;
	class Shader;
	class Surface;
}

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
		/* * * * * * * * * * * * * * * * * * * * */

		struct NoobFile : public ml::INonCopyable
		{
			using List = typename ml::List<NoobFile *>;

			enum { MaxName = 32, MaxData = 8192 };

			ml::String	name;
			char		data[MaxData];
			bool		open;
			bool		dirty;

			NoobFile(const ml::String & name, const ml::String & data)
				: name(name), data(), open(true), dirty(false)
			{
				std::strcpy(this->data, data.c_str());
			}
		};

		/* * * * * * * * * * * * * * * * * * * * */

		struct NoobsData : public ml::INonCopyable
		{
			NoobFile::List	files		= {};
			ml::Surface	*	surf_main	= nullptr;
			ml::Surface	*	surf_post	= nullptr;
			ml::Material *	material	= nullptr;
			ml::Entity *	entity		= nullptr;
			ml::Renderer *	renderer	= nullptr;
			float			deltaTime	= 0.f;
			float			totalTime	= 0.f;
			ml::vec2i		resolution	= { 1920, 1080 };
			bool			freeAspect	= true;
			ml::vec4f		clearColor	= ml::Color::Gray;
			bool			showScene	= true;
			bool			showBuilder	= true;

		} noobs;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_NOOBS_HPP_