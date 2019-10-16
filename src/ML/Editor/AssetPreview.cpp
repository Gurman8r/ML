#include <ML/Editor/AssetPreview.hpp>
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
		case typeof<Image>::hash:
			if (auto temp { static_cast<const Image *>(value) })
			{
				return createPreview(value, loadTemporary(*temp));
			}
			break;

		case typeof<Texture>::hash:
			if (auto temp { static_cast<Texture *>(value) })
			{
				switch (temp->sampler())
				{
				case GL::Texture2D: return createPreview(value, temp);
				}
			}
			break;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}