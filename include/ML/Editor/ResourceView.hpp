#ifndef _ML_RESOURCE_VIEW_HPP_
#define _ML_RESOURCE_VIEW_HPP_

#include <ML/Editor/GUI_Window.hpp>

#define ML_ResourceView ml::ResourceView::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ResourceView final
		: public GUI_Window
		, public ISingleton<ResourceView>
	{
		friend class ISingleton<ResourceView>;

	private:
		ResourceView();
		~ResourceView();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	private:
		struct Layout;

		void draw_surface_registry	();
		void draw_entity_registry	();
		void draw_font_registry		();
		void draw_image_registry	();
		void draw_lua_registry		();
		void draw_material_registry	();
		void draw_mesh_registry	();
		void draw_model_registry	();
		void draw_plugin_registry	();
		void draw_script_registry	();
		void draw_shader_registry	();
		void draw_skybox_registry	();
		void draw_sound_registry	();
		void draw_sprite_registry	();
		void draw_texture_registry	();
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCE_VIEW_HPP_