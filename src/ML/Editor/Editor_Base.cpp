#include <ML/Editor/Editor_Base.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Preferences.hpp>
#include <ML/Editor/EditorEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Base::Editor_Base(C_String title, C_String hotkey, bool startOpen)
		: m_title	{ title }
		, m_hotkey	{ hotkey }
		, m_open	{ startOpen }
		, m_good	{ false }
		, m_flags	{ ImGuiWindowFlags_None }
	{
		ML_EventSystem.addListener<GuiEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Base::onEvent(const Event & value)
	{
		switch (*value)
		{
		case GuiEvent::ID: if (auto ev{ value.as<GuiEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			if (this->isOpen()) { this->draw(); }

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	bool Editor_Base::beginDraw(int32_t flags)
	{
		ImGui::PushID((int32_t)typeof<>(*this).hash);
		ImGui::PushID(ML_ADDRESSOF(this));
		ImGui::PushID(getTitle());
		return (m_good = ImGui::Begin(m_title, &m_open, (m_flags = flags)));
	}

	bool Editor_Base::endDraw()
	{
		ImGui::End();
		ImGui::PopID();
		ImGui::PopID();
		ImGui::PopID();
		return m_good;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor_Base::setFocused(bool value)
	{
		if (setOpen(value))
		{
			ImGui::SetWindowFocus(getTitle());
			return true;
		}
		return false;
	}

	bool Editor_Base::drawMenuItem(bool showHotkey)
	{
		return ImGui::MenuItem(getTitle(), (showHotkey ? getHotkey() : nullptr), openPtr());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}