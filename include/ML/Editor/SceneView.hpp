#ifndef _ML_SCENE_VIEW_HPP_
#define _ML_SCENE_VIEW_HPP_

#include <ML/Editor/GUI_Window.hpp>
#include <ML/Graphics/Texture.hpp>

#define ML_SceneView ml::SceneView::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API SceneView final
		: public GUI_Window
		, public ISingleton<SceneView>
	{
		friend class ISingleton<SceneView>;

	private:
		SceneView();
		~SceneView();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	public:
		template <
			typename _Fun
		> inline bool drawFun(bool * p_open, _Fun fun)
		{
			if (drawGui(p_open))
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