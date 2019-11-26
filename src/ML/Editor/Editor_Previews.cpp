#include <ML/Editor/Editor_Previews.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/RenderTexture.hpp>
#include <ML/Core/EventSystem.hpp>
#include <ML/Window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Editor_Previews::Editor_Previews()
		: Editor_Widget { "Previews", "", false }
		, m_previewMap{}
		, m_textureList{}
	{
		ML_EventSystem.addListener<UnloadEvent>(this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Previews::onEvent(Event const & value)
	{
		Editor_Widget::onEvent(value);

		switch (*value)
		{
		case UnloadEvent::ID: if (auto ev{ value.as<UnloadEvent>() })
		{
			/* * * * * * * * * * * * * * * * * * * * */

			for (auto & elem : m_textureList)
			{
				if (elem) { delete elem; }
			}
			m_textureList.clear();
			m_previewMap.clear();

			/* * * * * * * * * * * * * * * * * * * * */
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Editor_Previews::draw()
	{
		return false;
		//ImGui::SetNextWindowSize({ 640, 480 }, ImGuiCond_FirstUseEver);
		//if (beginDraw(0))
		//{
		//}
		//return endDraw();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture const * Editor_Previews::getPreview(const typeof<> & type, void * value) const
	{
		if (!value) { return nullptr; }
		auto it{ m_previewMap.find(value) };
		if (it != m_previewMap.end()) { return it->second; }
		switch (type.hash())
		{
		case typeof<Glyph>::hash():
			if (auto temp{ static_cast<Glyph const *>(value) })
			{
				return getPreview(temp->texture);
			}
			break;

		case typeof<Image>::hash():
			if (auto temp{ static_cast<Image const *>(value) })
			{
				return insertPreview(value, loadTemp(*temp));
			}
			break;

		case typeof<RenderTexture>::hash():
			if (auto temp{ static_cast<RenderTexture const *>(value) })
			{
				return getPreview(temp->texture());
			}

		case typeof<Texture>::hash():
			if (auto temp{ static_cast<Texture const *>(value) })
			{
				switch (temp->sampler())
				{
				case GL::Texture2D: return insertPreview(value, temp);
				}
			}
			break;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Editor_Previews::drawPreview(const typeof<> & type, void * value, vec2 const & size, Clbk clbk) const
	{
		ImGui::PushID((int32_t)type.hash());
		ImGui::PushID(ML_ADDRESSOF(value));
		ImGui::PushID(value);
		if (auto preview{ getPreview(type, value) })
		{
			const vec2 dst
			{
				size[0] == 0.0f ? ImGuiExt::GetContentRegionAvail()[0] : size[0],
				size[1] == 0.0f ? ImGuiExt::GetContentRegionAvail()[1] : size[1]
			};

			const vec2 scl{ alg::scale_to_fit((vec2)preview->size(), dst) };

			const vec2 pos{ ((dst - scl) * 0.5f) };

			if (clbk) { clbk(); }

			ImGui::BeginChild(("##Editor_Previews##" + type.name().str() + "##").c_str(), { 0, 0 }, 0);

			ImGui::SetCursorPos({ pos[0], pos[1] });

			ImGui::Image(preview->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });

			ImGui::EndChild();
		}
		ImGui::PopID();
		ImGui::PopID();
		ImGui::PopID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}