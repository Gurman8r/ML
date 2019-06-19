#ifndef _ML_DOCKSPACE_GUI_HPP_
#define _ML_DOCKSPACE_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Core/Vector2.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API DockspaceGui final 
		: public EditorGui
	{
		friend class Editor;

	public:
		enum DockNode : int32_t
		{
			Root,
			Left,	LeftUp,		LeftDn,
			Mid,	MidUp,		MidDn,
			Right,	RightUp,	RightDn,

			MAX_DOCK_POS
		};

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit DockspaceGui(EventSystem & eventSystem);
		~DockspaceGui();

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

	public:
		inline uint32_t getNode(const int32_t i) const 
		{ 
			return (((i >= DockNode::Root) && (i < DockNode::MAX_DOCK_POS))
				? m_nodes[i]
				: NULL
			);
		}

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		float		m_border;
		vec2		m_padding;
		float		m_rounding;
		vec2		m_size;
		float		m_bgAlpha;
		uint32_t	m_nodes[MAX_DOCK_POS];
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DOCKSPACE_GUI_HPP_