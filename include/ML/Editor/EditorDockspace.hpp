#ifndef _ML_EDITOR_DOCKSPACE_HPP_
#define _ML_EDITOR_DOCKSPACE_HPP_

#include <ML/Editor/EditorWindow.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API EditorDockspace final : public EditorWindow
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;
		
		EditorDockspace();

		void update() override;

		bool beginDraw(int32_t flags) override;
		
		bool draw() override;
		
		bool endDraw() override;

	public:
		enum DockNode : int32_t
		{
			Root,
			Left, LeftUp, LeftDn,
			Right, RightUp, RightDn,

			MAX_DOCK_NODE
		};

		uint32_t beginBuilder(int32_t flags);
		uint32_t endBuilder(uint32_t root);
		uint32_t dockWindow(C_String name, uint32_t id);
		uint32_t splitNode(uint32_t id, int32_t dir, float_t ratio, uint32_t * other);
		uint32_t splitNode(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline uint32_t getNode(const int32_t i) const 
		{ 
			return (((i >= (int32_t)Root) && (i < (int32_t)MAX_DOCK_NODE))
				? m_nodes[i]
				: NULL
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		float_t		m_border;
		vec2		m_padding;
		float_t		m_rounding;
		vec2		m_size;
		float_t		m_bgAlpha;
		uint32_t	m_nodes[MAX_DOCK_NODE];
		int32_t		m_dockflags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_DOCKSPACE_HPP_