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

		void draw_effects	();
		void draw_entities	();
		void draw_fonts		();
		void draw_images	();
		void draw_lua		();
		void draw_materials	();
		void draw_meshes	();
		void draw_models	();
		void draw_plugins	();
		void draw_scripts	();
		void draw_shaders	();
		void draw_skyboxes	();
		void draw_sounds	();
		void draw_sprites	();
		void draw_textures	();
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCE_VIEW_HPP_