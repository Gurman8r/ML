#ifndef _ML_RESOURCE_VIEW_HPP_
#define _ML_RESOURCE_VIEW_HPP_

#include <ML/Editor/BaseWidget.hpp>
#include <ML/Engine/Resources.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ResourceView final
		: public BaseWidget
	{
		friend class Editor;

	private:
		ResourceView();
		~ResourceView();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(const GuiEvent * ev, bool * p_open) override;

	private:
		struct Layout;

		void draw_entity_registry	(Registry<Entity>		& entities	);
		void draw_font_registry		(Registry<Font>			& fonts		);
		void draw_image_registry	(Registry<Image>		& images	);
		void draw_lua_registry		(Registry<LuaScript>	& lua		);
		void draw_material_registry	(Registry<Material>		& materials	);
		void draw_mesh_registry		(Registry<Mesh>			& meshes	);
		void draw_model_registry	(Registry<Model>		& models	);
		void draw_plugin_registry	(Registry<Plugin>		& plugins	);
		void draw_script_registry	(Registry<Script>		& scripts	);
		void draw_shader_registry	(Registry<Shader>		& shaders	);
		void draw_skybox_registry	(Registry<Skybox>		& skyboxes	);
		void draw_sound_registry	(Registry<Sound>		& sounds	);
		void draw_sprite_registry	(Registry<Sprite>		& sprites	);
		void draw_surface_registry	(Registry<Surface>		& surfaces	);
		void draw_texture_registry	(Registry<Texture>		& textures	);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCE_VIEW_HPP_