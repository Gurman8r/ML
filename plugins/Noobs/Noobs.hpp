#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Engine/Asset.hpp>
#include <imgui/TextEditor.h>

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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit Noobs(EventSystem & eventSystem);
		
		~Noobs() {}
		
		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void onStart	(const StartEvent	& ev);
		void onUpdate	(const UpdateEvent	& ev);
		void onDraw		(const DrawEvent	& ev);
		void onGui		(const GuiEvent		& ev);
		void onExit		(const ExitEvent	& ev);

		// DEMO SKYBOX
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoSkybox final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			Asset<Model>	model		{ "default_skybox" };
			Asset<Material> material	{ "skybox" };

			DemoSkybox() = default;

			/* * * * * * * * * * * * * * * * * * * * */
		} m_skybox;


		// DEMO PIPELINE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum : size_t
		{
			Surf_Main, 
			Surf_Post,
			MAX_DEMO_SURFACE
		};
		struct DemoPipeline final : public I_NonCopyable, public Array<
			Asset<Surface>, MAX_DEMO_SURFACE
		>
		{
			/* * * * * * * * * * * * * * * * * * * * */

			DemoPipeline() : Array {
				Asset<Surface> { "surf_scene_main" },
				Asset<Surface> { "surf_scene_post" },
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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoScene final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			DemoScene() 
				: m_open		{ true }
				, m_autoView	{ true }
				, m_clearColor	{ Color::black }
				, m_viewport	{ 1920, 1080 }
				, m_effectMode	{ "u_effectMode", 3 }
				, m_kernel		{ "u_kernel", { 0, 0, 0, 0, 9, 0, 0, 0, 0 } }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * */

			void render(const GuiEvent & ev, C_String title, const Surface * surf);

			/* * * * * * * * * * * * * * * * * * * * */

			inline auto is_open()		-> bool &		{ return m_open; }
			inline auto autoView()		-> bool	&		{ return m_autoView; }
			inline auto clearColor()	-> vec4	&		{ return m_clearColor; }
			inline auto viewport()		-> vec2	&		{ return m_viewport; }
			inline auto effectMode()	-> uni_int	&	{ return m_effectMode; }
			inline auto kernel()		-> uni_mat3	&	{ return m_kernel; }

			/* * * * * * * * * * * * * * * * * * * * */

		private:
			bool		m_open;
			bool		m_autoView;
			vec4		m_clearColor;
			vec2		m_viewport;
			uni_int		m_effectMode;
			uni_mat3	m_kernel;

			/* * * * * * * * * * * * * * * * * * * * */
		} m_scene;


		// DEMO FILE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoFile final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			enum Type : size_t
			{
				Conf, Vert, Frag, Geom, 
				MAX_DEMO_FILE_TYPE
			};

			static constexpr C_String Names[MAX_DEMO_FILE_TYPE] = {
				"Config",
				"Vertex",
				"Fragment",
				"Geometry",
			};

			static constexpr C_String Tags[MAX_DEMO_FILE_TYPE] = {
				"",
				"#shader vertex",
				"#shader fragment",
				"#shader geometry",
			};

			static constexpr C_String Incl[MAX_DEMO_FILE_TYPE] = {
				"",
				"#include \"Vertex\"",
				"#include \"Fragment\"",
				"#include \"Geometry\"",
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

			void render(const GuiEvent & ev);

			/* * * * * * * * * * * * * * * * * * * * */
		};


		// DEMO EDITOR
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoEditor final : public I_Newable, public I_NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			using File_List = typename Array<DemoFile *, DemoFile::MAX_DEMO_FILE_TYPE>;

			/* * * * * * * * * * * * * * * * * * * * */

			DemoEditor() 
				: m_open	{ true }
				, m_material{ "demo_material" }
				, m_entity	{ "demo_entity" }
				, m_model	{ "sphere32x24" }
				, m_renderer{ nullptr }
				, m_files	{ nullptr }
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
			String	parse_sources(const String & source) const;

			/* * * * * * * * * * * * * * * * * * * * */

			inline auto is_open()	-> bool &				{ return m_open; }
			inline auto material()	-> Asset<Material> &	{ return m_material; }
			inline auto model()		-> Asset<Model> &		{ return m_model; }
			inline auto shader()	-> const Shader *&		{ return m_material->shader(); }
			inline auto entity()	-> Asset<Entity> &		{ return m_entity; }
			inline auto renderer()	-> Renderer *&			{ return m_renderer; }

			/* * * * * * * * * * * * * * * * * * * * */

		private:
			bool			m_open;
			Asset<Material> m_material;
			Asset<Model>	m_model;
			Asset<Entity>	m_entity;
			Renderer *		m_renderer;
			File_List		m_files;

			/* * * * * * * * * * * * * * * * * * * * */
		} m_editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NOOBS_HPP_