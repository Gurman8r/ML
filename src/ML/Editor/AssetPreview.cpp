#include <ML/Editor/AssetPreview.hpp>
#include <ML/Graphics/Surface.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	AssetPreview::AssetPreview()
		: m_previews {}
	{
	}

	AssetPreview::~AssetPreview() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool AssetPreview::dispose()
	{
		if (!m_previews.empty())
		{
			for (auto & pair : m_previews)
			{
				if (pair.second) { delete pair.second; }
			}
			m_previews.clear();
		}
		return m_previews.empty();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const Texture * AssetPreview::getPreview(const typeof<> & type, void * value) const
	{
		if (!value) { return false; }

		auto it { m_previews.find(value) };
		if (it != m_previews.end())
		{
			return it->second; 
		}

		switch (type.hash)
		{
		case typeof<Image>::hash:
			if (auto temp { static_cast<const Image *>(value) })
			{
				return m_previews.insert({ value, new Texture { *temp } }).first->second;
			}
			break;

		case typeof<Texture>::hash:
			if (auto temp { static_cast<Texture *>(value) })
			{
				switch (temp->sampler())
				{
				case GL::Texture2D:
					return m_previews.insert({ value, temp }).first->second;
				}
			}
			break;
		}
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}