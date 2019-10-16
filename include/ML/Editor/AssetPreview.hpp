#ifndef _ML_ASSET_PREVIEW_HPP_
#define _ML_ASSET_PREVIEW_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Graphics/Texture.hpp>

#define ML_AssetPreview ::ml::AssetPreview::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API AssetPreview final : public Singleton<AssetPreview>, Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Texture * getPreview(const typeof<> & type, void * value) const;

		template <class T> inline const Texture * getPreview(void * value) const
		{
			return getPreview(typeof<T>(), value);
		}

		template <class T> inline const Texture * getPreview(const T & value) const
		{
			return getPreview<T>(static_cast<void *>(std::remove_cv_t<T *>(&value)));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<AssetPreview>;

		AssetPreview();
		~AssetPreview();

		mutable HashMap<void *, Texture *> m_previews;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_PREVIEW_HPP_