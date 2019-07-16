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

		struct LoadTester final
			: public I_Disposable
			, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Thread	thr		{};
			Trigger trigger	{};
			
			int32_t numAttempt { 0 };
			int32_t numSuccess { 0 };
			int32_t numFailure { 0 };
			int32_t maxElement { 0 };

			/* * * * * * * * * * * * * * * * * * * * */

			LoadTester() {}

			~LoadTester() { dispose(); }

			/* * * * * * * * * * * * * * * * * * * * */

			inline bool isAvailable() const
			{
				return !isWorking() && !thr.alive();
			}

			inline bool isWorking() const 
			{ 
				return (maxElement > 0) && (numAttempt < maxElement);
			}
			
			inline bool isDone() const
			{ 
				return !isWorking() && (numAttempt > 0);
			}

			/* * * * * * * * * * * * * * * * * * * * */

			inline bool dispose() override
			{
				return reset().thr.dispose();
			}

			inline float_t progress() const
			{
				return ((isWorking()) ? (float_t)numAttempt / (float_t)maxElement : 0.0f);
			}

			/* * * * * * * * * * * * * * * * * * * * */

			inline LoadTester & attempt(bool success)
			{
				numSuccess += (int32_t)(success);
				numFailure += (int32_t)(!success);
				numAttempt++;
				return (*this);
			}

			inline LoadTester & init(int32_t maxValue)
			{
				reset().maxElement = maxValue;
				return (*this);
			}

			inline LoadTester & reset()
			{
				numAttempt = 0;
				numSuccess = 0;
				numFailure = 0;
				maxElement = 0;
				return (*this);
			}

			/* * * * * * * * * * * * * * * * * * * * */

		} loader;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_NOOBS_HPP_