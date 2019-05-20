#ifndef _ML_PROJECT_HPP_
#define _ML_PROJECT_HPP_

#include <ML/Editor/BaseWidget.hpp>
#include <ML/Engine/Resources.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Project final
		: public BaseWidget
	{
		friend class Editor;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		Project(Editor & editor, bool open);
		~Project();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const DrawGuiEvent & ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		struct Layout;

		void draw_entity_registry	(Resources & res, Registry<Entity>		& entities	);
		void draw_font_registry		(Resources & res, Registry<Font>		& fonts		);
		void draw_image_registry	(Resources & res, Registry<Image>		& images	);
		void draw_lua_registry		(Resources & res, Registry<LuaScript>	& lua		);
		void draw_material_registry	(Resources & res, Registry<Material>	& materials	);
		void draw_mesh_registry		(Resources & res, Registry<Mesh>		& meshes	);
		void draw_model_registry	(Resources & res, Registry<Model>		& models	);
		void draw_plugin_registry	(Resources & res, Registry<Plugin>		& plugins	);
		void draw_script_registry	(Resources & res, Registry<Script>		& scripts	);
		void draw_shader_registry	(Resources & res, Registry<Shader>		& shaders	);
		void draw_skybox_registry	(Resources & res, Registry<Skybox>		& skyboxes	);
		void draw_sound_registry	(Resources & res, Registry<Sound>		& sounds	);
		void draw_sprite_registry	(Resources & res, Registry<Sprite>		& sprites	);
		void draw_surface_registry	(Resources & res, Registry<Surface>		& surfaces	);
		void draw_texture_registry	(Resources & res, Registry<Texture>		& textures	);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROJECT_HPP_