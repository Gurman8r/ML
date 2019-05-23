#ifndef _ML_SCENE_VIEW_HPP_
#define _ML_SCENE_VIEW_HPP_

#include <ML/Editor/EditorWindow.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API SceneView final 
		: public EditorWindow
	{
		friend class Editor;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit SceneView(EventSystem & eventSystem, bool startOpen);
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
			if (m_open)
			{
				if (drawGui(ev))
				{
					fun();
				}
				return endDraw();
			}
			return false;
		}

		bool updateTexture(Texture * texture);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCENE_VIEW_HPP_