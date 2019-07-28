#ifndef _NOOBS_HPP_
#define _NOOBS_HPP_

#include <ML/Editor/EditorPlugin.hpp>
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

	struct ML_PLUGIN_API Noobs final : public EditorPlugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit Noobs(EventSystem & eventSystem);
		
		~Noobs() {}

		void onEvent(const Event * value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		void onStart	(const StartEvent	& ev) override;
		void onUpdate	(const UpdateEvent	& ev) override;
		void onDraw		(const DrawEvent	& ev) override;
		void onGui		(const GuiEvent		& ev) override;
		void onExit		(const ExitEvent	& ev) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// Surfaces
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum Surf_
		{
			Surf_Main, Surf_Post,

			MAX_DEMO_SURFACE
		};
		Array<Asset<Surface>, MAX_DEMO_SURFACE> surf
		{
			Asset<Surface> { "surf_scene_main" },
			Asset<Surface> { "surf_scene_post" },
		};

		// Demo Skybox
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoSkybox
		{
			Asset<Model>	model		{ "default_skybox" };
			Asset<Material> material	{ "skybox" };

			inline operator bool() const 
			{ 
				return model && material; 
			}

		} skybox;

		// Demo Scene
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoScene
		{
			bool		is_open		{ true };
			bool		autoView	{ true };
			vec4		clearColor	{ Color::black };
			vec2		viewport	{ 1920, 1080 };
			uni_int		effectMode	{ "u_effectMode", 3 };
			uni_mat3	kernel		{ "u_kernel", { 0, 0, 0, 0, 1, 0, 0, 0, 0 } };

			void Render(C_String title, const Surface * surf);

		} scene;

		// Demo Editor
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		struct DemoEditor
		{
			struct SourceFile : public I_NonCopyable
			{
				using TextEdit = typename ImGui::TextEditor;

				enum { MaxName = 32 };

				TextEdit	text;
				String		name;
				bool		open;
				bool		dirty;

				SourceFile(const String & name, const String & data)
					: name(name), open(true), dirty(false)
				{
					text.SetText(data);
					text.SetLanguageDefinition(TextEdit::LanguageDefinition::GLSL());
				}
			};

			using FileList = typename List<SourceFile *>;

			bool			is_open		{ true };
			Asset<Material> material	{ "noobs" };
			Asset<Model>	model		{ "sphere32x24"	};
			Asset<Entity>	entity		{ "noobs" };
			Renderer *		renderer	{ nullptr };
			FileList		files		{};

			void Render(C_String title);

			String	parseFiles(const FileList & file_list, const String & src) const;
			void	generateFiles();
			void	disposeFiles();

		} editor;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_NOOBS_HPP_