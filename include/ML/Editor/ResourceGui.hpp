#ifndef _ML_RESOURCE_GUI_HPP_
#define _ML_RESOURCE_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Engine/Resources.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct uni_base;

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

		void draw_entity_registry	(Resources & res, Registry<Entity>		& entities	);
		void draw_font_registry		(Resources & res, Registry<Font>		& fonts		);
		void draw_image_registry	(Resources & res, Registry<Image>		& images	);
		void draw_material_registry	(Resources & res, Registry<Material>	& materials	);
		void draw_mesh_registry		(Resources & res, Registry<Mesh>		& meshes	);
		void draw_model_registry	(Resources & res, Registry<Model>		& models	);
		void draw_script_registry	(Resources & res, Registry<Script>		& scripts	);
		void draw_shader_registry	(Resources & res, Registry<Shader>		& shaders	);
		void draw_sprite_registry	(Resources & res, Registry<Sprite>		& sprites	);
		void draw_surface_registry	(Resources & res, Registry<Surface>		& surfaces	);
		void draw_texture_registry	(Resources & res, Registry<Texture>		& textures	);

	public:
		static bool StringCombo(CString label, int32_t & index, const List<String> & keys);
		static void NewUniformPopup(Material * mat);
		static bool UniformField(Resources & resources, const String & label, uni_base * value);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCE_GUI_HPP_