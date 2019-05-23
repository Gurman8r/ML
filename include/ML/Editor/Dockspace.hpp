#ifndef _ML_DOCKSPACE_HPP_
#define _ML_DOCKSPACE_HPP_

#include <ML/Editor/EditorWindow.hpp>
#include <ML/Core/Vector2.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Dockspace final 
		: public EditorWindow
	{
		friend class Editor;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit Dockspace(EventSystem & eventSystem, bool startOpen);
		~Dockspace();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;
		bool beginDraw(int32_t flags = 0) override;
		bool endDraw() override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		uint32_t beginBuilder(int32_t flags);
		uint32_t endBuilder(uint32_t root);
		uint32_t dockWindow(CString name, uint32_t id);
		uint32_t splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * other);
		uint32_t splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * out, uint32_t * other);

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		float	m_border;
		vec2	m_padding;
		float	m_rounding;
		vec2	m_size;
		float	m_bgAlpha;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DOCKSPACE_HPP_