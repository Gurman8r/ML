#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Engine/Ref.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ImGuiColorTextEdit/TextEditor.h>

extern "C" ML_PLUGIN_API ml::Plugin * ML_Plugin_Main();

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
		
		Noobs();
		
		~Noobs() { }
		
		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void onStart	(const StartEvent	& ev);
		void onUpdate	(const UpdateEvent	& ev);
		void onDraw		(const DrawEvent	& ev);
		void onGui		(const GuiEvent		& ev);
		void onUnload	(const UnloadEvent	& ev);

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


		// ERROR
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct Error final : Newable
		{
			String	file;
			int32_t line;
			String	code;
			String	desc;

			Error(const String & file, int32_t line, const String & code, const String & expr)
				: file { file }
				, line { line }
				, code { code }
				, desc { expr }
			{
			}

			Error(const Error & copy) : Error {
				copy.file, copy.line, copy.code, copy.desc
			}
			{
			}

			inline operator bool() const
			{
				return file && line && code && desc;
			}

			inline friend ML_SERIALIZE(std::ostream & out, const Error & in)
			{
				return out
					<< "File: \'" << in.file << "\'" << endl
					<< "Line: \'" << in.line << "\'" << endl
					<< "Code: \'" << in.code << "\'" << endl
					<< "Desc: \'" << in.desc << "\'" << endl
					;
			}
		};

		static inline Error decode_error(uint32_t type, String str)
		{
			Error err { "", 0, "", "" };

			if (!str) { return err; }
			if (str.front() != '0') { return err; }
			str.erase(str.begin());

			switch (type)
			{
			case GL::FragmentShader: err.file = "Fragment"; break;
			case GL::GeometryShader: err.file = "Geometry"; break;
			case GL::VertexShader: err.file = "Vertex"; break;
			default: err.file = "Unknown"; break;
			}

			size_t a, b;
			if ((a = str.find_first_of('(')) != String::npos)
			{
				if ((b = str.find_first_of(')', a + 1)) != String::npos)
				{
					err.line = util::to_i32(str.substr(a + 1, b - a - 1));

					if ((a = str.find_first_of(':', b)) != String::npos)
					{
						if ((b = str.find_first_of(':', a + 1)) != String::npos)
						{
							err.code = String { str.substr(a + 2, b - a - 2) }.removeAll("error ");
							err.desc = str.substr(b + 2);
						}
					}
				}
			}

			return err;
		}



		// FILE
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		using TextEditor = ImGui::TextEditor;

		struct DemoFile final : public Newable, public NonCopyable
		{
			/* * * * * * * * * * * * * * * * * * * * */

			enum FileType : size_t { Frag, Vert, Geom, MAX_DEMO_FILE };

			static constexpr C_String Names[] = { "Fragment", "Vertex", "Geometry" };

			/* * * * * * * * * * * * * * * * * * * * */

			FileType	type;
			TextEditor	text;
			String		name;
			bool		open;
			bool		dirty;
			List<Error>	errs;

			/* * * * * * * * * * * * * * * * * * * * */

			DemoFile(FileType type, const String & text)
				: type { type }
				, text { text }
				, name { Names[type] }
				, open { false }
				, dirty { false }
				, errs {}
			{
				this->text.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
				this->text.SetShowWhitespaces(false);
				this->text.SetPalette(TextEditor::GetLightPalette());
			}

			/* * * * * * * * * * * * * * * * * * * * */
		};

		using FileList = Array<DemoFile *, DemoFile::MAX_DEMO_FILE>;


		// EDITOR
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool		m_editor_open	{ true };
		Ref<Entity>	m_entity		{};
		Renderer *	m_renderer		{ nullptr };
		FileList	m_files			{ 0 };
		bool		m_display_open	{ true };
		bool		m_freeAspect	{ true };
		vec2		m_viewport		{ 1920, 1080 };

		/* * * * * * * * * * * * * * * * * * * * */

		bool	compile_sources();
		void	draw_display(C_String title, const Surface * surf);
		void	draw_editor(C_String title);
		bool	dispose_files();
		void	generate_sources();
		void	reset_sources();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NOOBS_HPP_