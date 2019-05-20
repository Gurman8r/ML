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
		/* * * * * * * * * * * * * * * * * * * * */
		SceneView(Editor & editor, bool open);
		~SceneView();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;
		bool beginDraw(int32_t flags = 0) override;
		bool endDraw() override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class Fun
		> inline bool drawFun(const GuiEvent & ev, Fun && fun)
		{
			if (onGui(ev))
			{
				fun();
				return endDraw();
			}
		}

		bool updateTexture(Texture * texture);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCENE_VIEW_HPP_