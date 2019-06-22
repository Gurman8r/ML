#ifndef _ML_RESOURCE_GUI_HPP_
#define _ML_RESOURCE_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Engine/Content.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API ResourceGui final 
		: public EditorGui
	{
		friend class Editor;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit ResourceGui(EventSystem & eventSystem);
		~ResourceGui();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		struct Layout;

		void draw_entity_registry	();
		void draw_font_registry		();
		void draw_image_registry	();
		void draw_material_registry	();
		void draw_mesh_registry		();
		void draw_model_registry	();
		void draw_script_registry	();
		void draw_shader_registry	();
		void draw_sprite_registry	();
		void draw_surface_registry	();
		void draw_texture_registry	();

	public:
		static void NewUniformPopup(Material * mat);
		static int32_t UniformField(const String & label, Uniform * value, bool drag = true);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCE_GUI_HPP_