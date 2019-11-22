#include <ML/Editor/Editor_Dockspace.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Dockspace::Editor_Dockspace()
		: Editor_Widget { "Dockspace", "", true }
		, m_border		{ 0.0f }
		, m_padding		{ 0.f, 0.f }
		, m_rounding	{ 0.0f }
		, m_size		{ 0.f, 0.f }
		, m_bgAlpha		{ 1.0f }
		, m_dockflags	{ ImGuiDockNodeFlags_AutoHideTabBar }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Dockspace::onEvent(Event const & value)
	{
		Editor_Widget::onEvent(value);
	}

	bool Editor_Dockspace::beginDraw(int32_t flags)
	{
		if (m_good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		{
			// Bounds
			ImGuiViewport const * viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			// Style
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, m_rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, m_border);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { m_padding[0], m_padding[1] });
			ImGui::SetNextWindowBgAlpha(m_bgAlpha);

			// Begin
			if (Editor_Widget::beginDraw(flags))
			{
				ImGui::PopStyleVar(3);
			}
		}
		return m_good;
	}

	bool Editor_Dockspace::draw()
	{
		if (this->beginDraw(
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground |
			(ML_Editor.mainMenuBar().isOpen() ? ImGuiWindowFlags_MenuBar : 0)
		))
		{
			if (m_nodes[Root] = beginBuilder(m_dockflags))
			{
				m_nodes[Left]	= splitNode(m_nodes[Root],	ImGuiDir_Left,	0.4f,	&m_nodes[Root]);
				m_nodes[Right]	= splitNode(m_nodes[Root],	ImGuiDir_Right, 0.6f,	&m_nodes[Root]);
				m_nodes[LeftUp]	= splitNode(m_nodes[Left],	ImGuiDir_Up,	0.75f,	&m_nodes[Left]);
				m_nodes[RightUp]= splitNode(m_nodes[Right], ImGuiDir_Up,	0.75f,	&m_nodes[Right]);
				m_nodes[LeftDn]	= splitNode(m_nodes[Left],	ImGuiDir_Down,	0.25f,	&m_nodes[Left]);
				m_nodes[RightDn]= splitNode(m_nodes[Right],	ImGuiDir_Down,	0.25f,	&m_nodes[Right]);

				ML_EventSystem.fireEvent<DockspaceEvent>(*this);

				endBuilder(m_nodes[Root]);
			};
		}
		return this->endDraw();
	}

	bool Editor_Dockspace::endDraw()
	{
		if (m_good)
		{
			ImGui::DockSpace(
				ImGui::GetID(getTitle()),
				{ m_size[0], m_size[1] },
				ImGuiDockNodeFlags_PassthruCentralNode | 
				m_dockflags
			);
		}
		return Editor_Widget::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	uint32_t Editor_Dockspace::beginBuilder(int32_t flags)
	{
		if (uint32_t root { ImGui::GetID(getTitle()) })
		{
			if (!ImGui::DockBuilderGetNode(root))
			{
				ImGui::DockBuilderRemoveNode(root);
				ImGui::DockBuilderAddNode(root, flags);
				return root;
			}
		}
		return NULL;
	}

	uint32_t Editor_Dockspace::endBuilder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t Editor_Dockspace::dockWindow(C_String name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t Editor_Dockspace::splitNode(uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		return splitNode(id, dir, ratio, nullptr, other);
	}

	uint32_t Editor_Dockspace::splitNode(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}