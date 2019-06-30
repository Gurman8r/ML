#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Graphics/RenderBatch.hpp>
#include <imgui/TextEditor.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct Entity;
	struct Material;
	struct Renderer;
	struct Surface;
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

		struct NoobFile : public ml::I_NonCopyable
		{
			using List = typename ml::List<NoobFile *>;

			enum { MaxName = 32 };

			ImGui::TextEditor	text;
			ml::String			name;
			bool				open;
			bool				dirty;

			NoobFile(const ml::String & name, const ml::String & data)
				: name(name), open(true), dirty(false)
			{
				text.SetText(data);
				text.SetLanguageDefinition(
					ImGui::TextEditor::LanguageDefinition::GLSL()
				);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * */

		struct NoobsData final : public ml::I_NonCopyable
		{
			// Content
			ml::Surface	*	surf_main	= nullptr;
			ml::Surface	*	surf_post	= nullptr;
			ml::Material *	material	= nullptr;
			ml::Entity *	entity		= nullptr;
			ml::Renderer *	renderer	= nullptr;

			// GUI Settings
			bool			showBuilder	= true;
			bool			showScene	= true;
			bool			freeAspect	= true;
			NoobFile::List	file_list	= {};
			ml::vec4f		clearColor	= ml::Color::black;
			ml::vec2		resolution	= { 1920, 1080 };

		} noobs;

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_NOOBS_HPP_