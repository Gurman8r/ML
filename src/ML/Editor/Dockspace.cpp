#include <ML/Editor/Dockspace.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/GUI.hpp>
#include <ML/Editor/ImGui.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dockspace::Dockspace(Editor & editor, bool open)
		: BaseWidget("Dockspace", editor, open)
		, m_border	(0.0f)
		, m_padding	(vec2::Zero)
		, m_rounding(0.0f)
		, m_size	(vec2::Zero)
		, m_bgAlpha	(1.0f)
	{
	}

	Dockspace::~Dockspace() {}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dockspace::drawGui(const DrawGuiEvent & ev)
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
			ImGuiWindowFlags_NoBackground))
		{
			// Dockspace Builder
			if (uint32_t root = beginBuilder(ImGuiDockNodeFlags_None))
			{
				uint32_t left		= splitNode(root, ImGuiDir_Left, 0.30f, &root);
				uint32_t center		= splitNode(root, ImGuiDir_Right, 0.5f, &root);
				uint32_t right		= splitNode(center, ImGuiDir_Right, 0.20f, &center);

				uint32_t left_U		= splitNode(left, ImGuiDir_Up, 0.65f, &left);
				uint32_t left_D		= splitNode(left, ImGuiDir_Down, 0.35f, &left);
				
				uint32_t center_U	= splitNode(center, ImGuiDir_Up, 0.65f, &center);
				uint32_t center_D	= splitNode(center, ImGuiDir_Down, 0.35f, &center);
				uint32_t center_DR	= splitNode(center, ImGuiDir_Right, 0.25f, &center_D);
				
				uint32_t right_U	= splitNode(right, ImGuiDir_Up, 0.65f, &right);
				uint32_t right_D	= splitNode(right, ImGuiDir_Down, 0.35f, &right);

				dockWindow(ev.editor.profiler.getTitle(),	left_U);
				dockWindow(ev.editor.network.getTitle(),	left_U);
				dockWindow(ev.editor.browser.getTitle(),	left_U);
				dockWindow(ev.editor.project.getTitle(),	left_U);
				dockWindow(ev.editor.terminal.getTitle(),	left_D);
				dockWindow(ev.editor.sceneView.getTitle(),	center_U);
				dockWindow(ev.editor.textEdit.getTitle(),	center_D);
				dockWindow(ev.editor.builder.getTitle(),	center_D);
				dockWindow(ev.editor.inspector.getTitle(),	center_DR);

				endBuilder(root);
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

			// Begi
			if (BaseWidget::beginDraw(flags))
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
				getID(),
				{ m_size[0], m_size[1] },
				ImGuiDockNodeFlags_PassthruDockspace
			);
		}
		return BaseWidget::endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t Dockspace::beginBuilder(int32_t flags)
	{
		if (uint32_t root = getID())
		{
			if (!ImGui::DockBuilderGetNode(root))
			{
				ImGui::DockBuilderRemoveNode(root);
				ImGui::DockBuilderAddNode(root, flags);
				return root;
			}
		}
		return 0;
	}

	uint32_t Dockspace::endBuilder(uint32_t root)
	{
		if (root)
		{
			ImGui::DockBuilderFinish(root);
		}
		return root;
	}

	uint32_t Dockspace::dockWindow(CString name, uint32_t id)
	{
		if (name && id)
		{
			ImGui::DockBuilderDockWindow(name, id);
			return id;
		}
		return 0;
	}

	uint32_t Dockspace::splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * other)
	{
		return splitNode(id, dir, ratio, NULL, other);
	}

	uint32_t Dockspace::splitNode(uint32_t id, int32_t dir, float ratio, uint32_t * out, uint32_t * other)
	{
		return ImGui::DockBuilderSplitNode(id, dir, ratio, out, other);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}