#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Graphics/Material.hpp>
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

		// DEMO SKYBOX
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoSkybox final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			bool enabled { false };

			Ref<Model>		model		{ "skybox" };
			Ref<Material>	material	{ "skybox" };

			DemoSkybox() = default;

			/* * * * * * * * * * * * * * * * * * * * */
		};


		// DEMO PIPELINE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum : size_t
		{
			Surf_Main, 
			Surf_Post,
			MAX_DEMO_SURFACE
		};
		struct DemoPipeline final : public I_NonCopyable, public Array<
			Ref<Surface>, MAX_DEMO_SURFACE
		>
		{
			/* * * * * * * * * * * * * * * * * * * * */

			DemoPipeline() : Array {
				Ref<Surface> { "surf_scene_main" },
				Ref<Surface> { "surf_scene_post" },
			}
			{
			}

			template <
				class Fun, class ... Args
			> inline void render_to(size_t i, Fun && fun, Args && ... args)
			{
				if (Surface * surf { (*this)[i] })
				{
					surf->render_to(fun, std::forward<Args>(args)...);
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */
		} m_pipeline;


		// DEMO SCENE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoScene final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			DemoScene() 
				: m_open		{ true }
				, m_freeAspect	{ true }
				, m_clearColor	{ Color::black }
				, m_viewport	{ 1920, 1080 }
				, m_skybox		{}
				, m_effectMode	{ "u_effectMode", 0 }
				, m_kernel		{ "u_kernel", { -1, -1, -1, -1, 9, -1, -1, -1, -1 } }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */

			void render(const GuiEvent & ev, C_String title, const Surface * surf);

			/* * * * * * * * * * * * * * * * * * * * */

			bool		m_open;
			bool		m_freeAspect;
			vec4		m_clearColor;
			vec2		m_viewport;
			DemoSkybox	m_skybox;
			uni_int		m_effectMode;
			uni_mat3	m_kernel;

			/* * * * * * * * * * * * * * * * * * * * */
		};


		// DEMO FILE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoFile final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			enum Type : size_t
			{
				Vert, Frag, Geom, 
				MAX_DEMO_FILE_TYPE
			};

			static constexpr C_String Names[MAX_DEMO_FILE_TYPE] = {
				"Vertex",
				"Fragment",
				"Geometry",
			};

			/* * * * * * * * * * * * * * * * * * * * */

			using TextEditor = ImGui::TextEditor;

			const Type	type;
			TextEditor	text;
			String		name;
			bool		open;
			bool		dirty;

			/* * * * * * * * * * * * * * * * * * * * */

			DemoFile(Type type, const String & data)
				: type	{ type }
				, text	{ data, TextEditor::LanguageDefinition::GLSL() }
				, name	{ Names[type] }
				, open	{ false }
				, dirty { false }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */
		};


		// DEMO EDITOR
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoEditor final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using File_List = typename Array<DemoFile *, DemoFile::MAX_DEMO_FILE_TYPE>;

			/* * * * * * * * * * * * * * * * * * * * */

			DemoScene scene;

			DemoEditor() 
				: m_open	{ true }
				, m_material{ "demo_material" }
				, m_entity	{ "demo_entity" }
				, m_model	{ "sphere32x24" }
				, m_renderer{ nullptr }
				, m_files	{ nullptr }
				, scene {}
			{
			}

			~DemoEditor() 
			{ 
				for (size_t i = 0; i < m_files.size(); i++)
				{
					if (m_files[i])
					{
						delete m_files[i];
					}
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			void	render(const GuiEvent & ev, C_String title);
			void	generate_sources();
			bool	compile_sources();
			void	reset_sources();

			/* * * * * * * * * * * * * * * * * * * * */

			inline auto is_open()	-> bool &			{ return m_open; }
			inline auto material()	-> Ref<Material> &	{ return m_material; }
			inline auto model()		-> Ref<Model> &		{ return m_model; }
			inline auto shader()	-> const Shader *&	{ return m_material->shader(); }
			inline auto entity()	-> Ref<Entity> &	{ return m_entity; }
			inline auto renderer()	-> Renderer *&		{ return m_renderer; }

			/* * * * * * * * * * * * * * * * * * * * */

			bool			m_open;
			Ref<Material>	m_material;
			Ref<Model>		m_model;
			Ref<Entity>		m_entity;
			Renderer *		m_renderer;
			File_List		m_files;

			/* * * * * * * * * * * * * * * * * * * * */
		} m_editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NOOBS_HPP_