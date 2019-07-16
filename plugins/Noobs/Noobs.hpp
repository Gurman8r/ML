#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Editor/EditorPlugin.hpp>
#include <ML/Graphics/Color.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Thread.hpp>
#include <ML/Core/Trigger.hpp>
#include <ML/Graphics/RenderBatch.hpp>
#include <imgui/TextEditor.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Entity;
	struct Material;
	struct Model;
	struct Renderer;
	struct Surface;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API Noobs final : public EditorPlugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit Noobs(EventSystem & eventSystem);
		
		~Noobs() {}

		void onEvent(const Event * value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void onEnter	(const EnterEvent	& ev) override;
		void onStart	(const StartEvent	& ev) override;
		void onUpdate	(const UpdateEvent	& ev) override;
		void onDraw		(const DrawEvent	& ev) override;
		void onGui		(const GuiEvent		& ev) override;
		void onExit		(const ExitEvent	& ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct NoobsFile : public I_NonCopyable
		{
			using TextEdit = typename ImGui::TextEditor;

			enum { MaxName = 32 };

			TextEdit	text;
			String		name;
			bool		open;
			bool		dirty;

			NoobsFile(const String & name, const String & data)
				: name(name), open(true), dirty(false)
			{
				text.SetText(data);
				text.SetLanguageDefinition(
					ImGui::TextEditor::LanguageDefinition::GLSL()
				);
			}
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using FileList = typename List<NoobsFile *>; // list of 'NoobsFile's

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct NoobsData final : public I_NonCopyable
		{
			// Content
			Surface	*	surf_main	{ nullptr };
			Surface	*	surf_post	{ nullptr };
			Material *	material	{ nullptr };
			Entity *	entity		{ nullptr };
			Renderer *	renderer	{ nullptr };

			// GUI Settings
			bool		showBuilder	{ true };
			bool		showScene	{ true };
			bool		freeAspect	{ true };
			FileList	files		{};
			vec4		clearColor	{ Color::black };
			vec2		resolution	{ 1920, 1080 };

		} noobs;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct LoadTester : public I_NonCopyable
		{
			Trigger trigger		{};
			Thread	thr			{};
			bool	isLoading	{ false };
			int32_t numLoaded	{ 0 };
			int32_t maxObjects	{ 0 };
		} loader;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_NOOBS_HPP_