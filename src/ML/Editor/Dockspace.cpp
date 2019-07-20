#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace(EventSystem & eventSystem)
		: EditorGui	(eventSystem, "Dockspace")
		, m_border	(0.0f)
		, m_padding	(vec2 { 0.f, 0.f })
		, m_rounding(0.0f)
		, m_size	(vec2 { 0.f, 0.f })
		, m_bgAlpha	(1.0f)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dockspace::drawGui(const GuiEvent & ev)
	{
		if (beginDraw(
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground
		))
		{
			if (m_nodes[Root] = beginBuilder(ImGuiDockNodeFlags_None))
			{
				m_nodes[Left]	= splitNode(m_nodes[Root],	ImGuiDir_Left,	0.50f,	&m_nodes[Root]);
				m_nodes[Right]	= splitNode(m_nodes[Root],	ImGuiDir_Right, 0.50f,	&m_nodes[Root]);
				m_nodes[LeftUp]	= splitNode(m_nodes[Left],	ImGuiDir_Up,	0.75f,	&m_nodes[Left]);
				m_nodes[RightUp]= splitNode(m_nodes[Right], ImGuiDir_Up,	0.75f,	&m_nodes[Right]);
				m_nodes[LeftDn]	= splitNode(m_nodes[Left],	ImGuiDir_Down,	0.25f,	&m_nodes[Left]);
				m_nodes[RightDn]= splitNode(m_nodes[Right],	ImGuiDir_Down,	0.25f,	&m_nodes[Right]);

				eventSystem().fireEvent(BuildDockspaceEvent(
					(*this)
				));

				endBuilder(m_nodes[Root]);
			};
		}
		return endDraw();
	}

	bool Dockspace::beginDraw(int32_t flags)
	{
		if (m_good = (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		{
			// Bounds
			const ImGuiViewport * viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			// Style
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, m_rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, m_border);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { m_padding[0], m_padding[1] });
			ImGui::SetNextWindowBgAlpha(m_bgAlpha);

			// Begin
			if (EditorGui::beginDraw(flags))
			{
				ImGui::PopStyleVar(3);
			}
		}
		return m_good;
	}

	bool Dockspace::endDraw()
	{
		if (m_good)
		{
			ImGui::DockSpace(
				ImGui::GetID(getTitle()),
				{ m_size[0], m_size[1] },
				ImGuiDockNodeFlags_PassthruCentralNode
			);
		}
		return EditorGui::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t Dockspace::beginBuilder(int32_t flags)
	{
		if (uint32_t root = ImGui::GetID(getTitle()))
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

	uint32_t Dockspace::endBuilder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t Dockspace::dockWindow(C_String name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return NULL;
	}

	uint32_t Dockspace::splitNode(uint32_t id, int32_t dir, float_t ratio, uint32_t * other)
	{
		return splitNode(id, dir, ratio, nullptr, other);
	}

	uint32_t Dockspace::splitNode(uint32_t id, int32_t dir, float_t ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}