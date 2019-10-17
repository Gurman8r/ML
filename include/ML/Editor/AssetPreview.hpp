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

		template <class T> inline const Texture * getPreview(const T * value) const
		{
			return getPreview<T>(std::remove_cv_t<void *>(value));
		}

		template <class T> inline const Texture * getPreview(const T & value) const
		{
			return getPreview<T>(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clbk = std::function<void()>;

		void drawPreview(const typeof<> & type, void * value, const vec2 & size, Clbk && fun) const;

		template <
			class T, class F
		> inline auto drawPreview(const T * value, const vec2 & size, F && fun) const
		{
			return drawPreview(typeof<T>(), std::remove_cv_t<void *>(value), size, fun);
		}

		template <
			class T, class F
		> inline auto drawPreview(const T & value, const vec2 & size, F && fun) const
		{
			return drawPreview<T>(&value, size, fun);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<AssetPreview>;

		AssetPreview() : m_previewMap {}, m_textureList {} {}
		~AssetPreview() { dispose(); }

		using PreviewMap = typename HashMap<void *, const Texture *>;
		using TextureList = typename List<Texture *>;

		mutable PreviewMap	m_previewMap;
		mutable TextureList m_textureList;

		template <class ... Args> inline Texture * loadTemp(Args && ... args) const
		{
			m_textureList.push_back(new Texture { std::forward<Args>(args)... });
			return m_textureList.back();
		}

		inline const Texture * insertPreview(void * value, const Texture * preview) const
		{
			return m_previewMap.insert({ value, preview }).first->second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_PREVIEW_HPP_