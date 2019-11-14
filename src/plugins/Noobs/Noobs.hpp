#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Engine/Engine.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ImGuiColorTextEdit/TextEditor.h>

extern "C" ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main();

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Entity;
	struct Material;
	struct Model;
	struct Renderer;
	struct Surface;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API Noobs final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Noobs();
		
		~Noobs() { }
		
		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		// PIPELINE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum : size_t { Surf_Main, Surf_Post, MAX_DEMO_SURFACE };

		using DemoPipeline = typename Array<ptr_t<Surface>, MAX_DEMO_SURFACE>;


		// ERRORS
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ShaderError final : public Newable
		{
			String	file;
			int32_t line;
			String	code;
			String	desc;

			ShaderError(const String & file, int32_t line, const String & code, const String & expr)
				: file { file }
				, line { line }
				, code { code }
				, desc { expr }
			{
			}

			ShaderError(const ShaderError & copy) 
				: ShaderError { copy.file, copy.line, copy.code, copy.desc }
			{
			}

			explicit ShaderError(uint32_t type, String str)
				: ShaderError { "", 0, "", "" }
			{
				if (!str || str.front() != '0') { return; }
				str.erase(str.begin());
				switch (type)
				{
				case GL::FragmentShader: this->file = "Fragment"; break;
				case GL::GeometryShader: this->file = "Geometry"; break;
				case GL::VertexShader: this->file = "Vertex"; break;
				default: this->file = "Unknown"; break;
				}
				size_t a, b;
				if ((a = str.find_first_of('(')) != String::npos)
				{
					if ((b = str.find_first_of(')', a + 1)) != String::npos)
					{
						this->line = util::to_i32(str.substr(a + 1, b - a - 1));

						if ((a = str.find_first_of(':', b)) != String::npos)
						{
							if ((b = str.find_first_of(':', a + 1)) != String::npos)
							{
								this->code = str.substr(a + 2, b - a - 2);
								this->desc = str.substr(b + 2);
							}
						}
					}
				}
			}
		};


		// FILES
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using TextEditor = ImGui::TextEditor;

		struct ShaderFile final : public Newable, public NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			enum FileType : size_t { Frag, Vert, Geom, MAX_DEMO_FILE };

			static constexpr C_String Names[] = { "Fragment", "Vertex", "Geometry" };

			using Errors = typename List<ShaderError>;

			/* * * * * * * * * * * * * * * * * * * * */

			FileType	type;
			TextEditor	text;
			String		name;
			bool		open;
			bool		dirty;
			Errors		errs;

			/* * * * * * * * * * * * * * * * * * * * */

			ShaderFile(FileType type)
				: type	{ type }
				, text	{}
				, name	{ Names[type] }
				, open	{ false }
				, dirty { false }
				, errs	{}
			{
				this->text.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
#if (!ML_DEBUG)
				this->text.SetPalette(TextEditor::GetLightPalette());
#endif
			}

			/* * * * * * * * * * * * * * * * * * * * */
		};

		using FileArray = Array<ptr_t<ShaderFile>, ShaderFile::MAX_DEMO_FILE>;


		// EDITOR
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum class DisplayMode : int32_t { Automatic, Manual, Fixed };

		bool m_editor_open		{ true };
		bool m_display_open		{ true };
		bool m_use_main_camera	{ true };

		DemoPipeline	m_pipeline		{ 0 };
		ptr_t<Entity>	m_entity		{ 0 };
		String			m_ent_name		{ "" };
		FileArray		m_files			{ 0 };
		DisplayMode		m_displayMode	{ 0 };
		int32_t			m_displayIndex	{ 0 };

		static constexpr auto display_name { "Display##Noobs##DemoView" };
		static constexpr auto editor_name { "Editor##Noobs##DemoEditor" };

		/* * * * * * * * * * * * * * * * * * * * */

		void draw_display(C_String title, const Surface * surf);
		void draw_editor(C_String title);

		void draw_code_tab();
		void draw_uniforms_tab();
		void draw_settings_tab();
		
		void draw_plugin_menu();

		Noobs & compile_sources();
		Noobs & dispose_files();
		Noobs & generate_sources();
		Noobs & reset_sources();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NOOBS_HPP_