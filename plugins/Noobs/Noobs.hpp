#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Editor/ImGui.hpp>
#include <imgui/addons/ImGuiColorTextEdit/TextEditor.h>

extern "C"
{
	ML_PLUGIN_API ml::Plugin * ML_Plugin_Main(ml::EventSystem & eventSystem);
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Entity;
	struct Material;
	struct Model;
	struct Renderer;
	struct Surface;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API Noobs final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit Noobs(EventSystem & eventSystem);
		
		~Noobs() {}
		
		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void onStart	(const StartEvent	& ev);
		void onUpdate	(const UpdateEvent	& ev);
		void onDraw		(const DrawEvent	& ev);
		void onGui		(const GuiEvent		& ev);
		void onExit		(const ExitEvent	& ev);

		// PIPELINE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum : size_t 
		{
			Surf_Main, 
			Surf_Post, 
			MAX_DEMO_SURFACE
		};
		
		Array<Ref<Surface>, MAX_DEMO_SURFACE> m_pipeline 
		{
			Ref<Surface> { "surf_scene_main" },
			Ref<Surface> { "surf_scene_post" },
		};


		// SCENE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoScene final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			bool m_open;
			bool m_freeAspect;
			vec4 m_clearColor;
			vec2 m_viewport;

			/* * * * * * * * * * * * * * * * * * * * */

			DemoScene() 
				: m_open		{ true }
				, m_freeAspect	{ true }
				, m_clearColor	{ Color::black }
				, m_viewport	{ 1920, 1080 }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */

			void render(const GuiEvent & ev, C_String title, const Surface * surf);

			/* * * * * * * * * * * * * * * * * * * * */
		} m_scene;


		// FILE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoFile final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			enum Type : size_t
			{
				Vert, Frag, Geom, MAX_DEMO_FILE
			};

			static constexpr C_String Names[MAX_DEMO_FILE] = {
				"Vertex", "Fragment", "Geometry",
			};

			/* * * * * * * * * * * * * * * * * * * * */

			using TextEditor = ImGui::TextEditor;

			const Type	type;
			TextEditor	text;
			String		name;
			bool		open;
			bool		dirty;

			/* * * * * * * * * * * * * * * * * * * * */

			DemoFile(Type type, const String & text)
				: type	{ type }
				, text	{ text }
				, name	{ Names[type] }
				, open	{ false }
				, dirty { false }
			{
				this->text.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
				this->text.SetShowWhitespaces(false);
				this->text.SetPalette(TextEditor::GetLightPalette());
			}

			/* * * * * * * * * * * * * * * * * * * * */
		};


		// EDITOR
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoEditor final 
			: public I_Newable
			, public I_NonCopyable
			, public I_Disposable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using File_List = typename Array<DemoFile *, DemoFile::MAX_DEMO_FILE>;

			/* * * * * * * * * * * * * * * * * * * * */

			bool			m_open		{ true };
			Ref<Entity>		m_entity	{};
			Ref<Material>	m_material	{};
			Ref<Model>		m_model		{};
			Renderer *		m_renderer	{ nullptr };
			File_List		m_files		{ nullptr };
			DemoScene		m_scene		{};
			Camera			m_camera	{};

			/* * * * * * * * * * * * * * * * * * * * */

			DemoEditor() {}
			~DemoEditor() { dispose(); }

			/* * * * * * * * * * * * * * * * * * * * */

			void	render(const GuiEvent & ev, C_String title);
			bool	dispose() override;
			void	generate_sources();
			bool	compile_sources();
			void	reset_sources();

			/* * * * * * * * * * * * * * * * * * * * */
		} m_editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NOOBS_HPP_