#include <ML/Editor/AssetPreview.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Surface.hpp>

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

	const Texture * AssetPreview::getPreview(const typeof<> & type, void * value) const
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
			if (auto temp { static_cast<const Image *>(value) })
			{
				return insertPreview(value, loadTemp(*temp));
			}
			break;

		case typeof<Surface>::hash:
			if (auto temp { static_cast<const Surface *>(value) })
			{
				return getPreview(temp->texture());
			}

		case typeof<Texture>::hash:
			if (auto temp { static_cast<const Texture *>(value) })
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
}