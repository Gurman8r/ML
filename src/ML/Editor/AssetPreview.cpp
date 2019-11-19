#include <ML/Editor/AssetPreview.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Editor/ImGui.hpp>
#include <ML/Editor/ImGuiExt.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool AssetPreview::dispose()
	{
		for (auto & elem : m_textureList)
		{
			if (elem) { delete elem; }
		}
		m_textureList.clear();
		m_previewMap.clear();
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture const * AssetPreview::getPreview(const typeof<> & type, void * value) const
	{
		if (!value) { return nullptr; }
		auto it { m_previewMap.find(value) };
		if (it != m_previewMap.end()) { return it->second; }
		switch (type.hash)
		{
		case typeof<Glyph>::hash:
			if (auto temp { static_cast<const Glyph *>(value) })
			{
				return getPreview(temp->texture);
			}
			break;

		case typeof<Image>::hash:
			if (auto temp { static_cast<Image const *>(value) })
			{
				return insertPreview(value, loadTemp(*temp));
			}
			break;

		case typeof<Surface>::hash:
			if (auto temp { static_cast<Surface const *>(value) })
			{
				return getPreview(temp->texture());
			}

		case typeof<Texture>::hash:
			if (auto temp { static_cast<Texture const *>(value) })
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

	void AssetPreview::drawPreview(const typeof<> & type, void * value, const vec2 & size, Clbk clbk) const
	{
		ImGui::PushID((int32_t)type.hash);
		ImGui::PushID(value);
		if (auto preview { getPreview(type, value) })
		{
			const vec2 dst 
			{ 
				size[0] == 0.0f ? ImGuiExt::GetContentRegionAvail()[0] : size[0],
				size[1] == 0.0f ? ImGuiExt::GetContentRegionAvail()[1] : size[1]
			};

			const vec2 scl { alg::scale_to_fit((vec2)preview->size(), dst) * 0.975f };

			const vec2 pos { ((dst - scl) * 0.5f) };

			if (clbk) { clbk(); }
			
			ImGui::BeginChild(
				("##AssetPreview##" + type.name.str() + "##").c_str(), 
				{ 0, 0 }, 
				true
			);
			
			ImGui::SetCursorPos({ pos[0], pos[1] });
			
			ImGui::Image(preview->get_address(), { scl[0], scl[1] }, { 0, 1 }, { 1, 0 });
			
			ImGui::EndChild();
		}
		ImGui::PopID();
		ImGui::PopID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}