#ifndef _ML_SCENE_VIEW_HPP_
#define _ML_SCENE_VIEW_HPP_

#include <ML/Editor/BaseWidget.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API SceneView final
		: public BaseWidget
	{
		friend class Editor;

	private:
		SceneView(Editor & editor, bool open = true);
		~SceneView();

	public:
		bool drawGui(const GuiEvent & ev) override;

	public:
		template <
			class Fun
		> inline bool drawFun(const GuiEvent & ev, Fun && fun)
		{
			if (drawGui(ev))
			{
				fun();
			}
			return endDraw();
		}

	protected:
		bool beginDraw(bool * p_open, int32_t flags) override;
		bool endDraw() override;

	public:
		bool updateTexture(Texture * texture);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCENE_VIEW_HPP_