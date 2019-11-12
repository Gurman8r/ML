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

		const_ptr_t<Texture> getPreview(const typeof<> & type, ptr_t<void> value) const;

		template <class T> inline const_ptr_t<Texture> getPreview(ptr_t<void> value) const
		{
			return getPreview(typeof<T>(), value);
		}

		template <class T> inline const_ptr_t<Texture> getPreview(const T * value) const
		{
			return getPreview<T>(std::remove_cv_t<ptr_t<void>>(value));
		}

		template <class T> inline const_ptr_t<Texture> getPreview(const T & value) const
		{
			return getPreview<T>(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clbk = std::function<void()>;

		void drawPreview(const typeof<> & type, ptr_t<void> value, const vec2 & size, Clbk fun) const;

		template <
			class T, class F
		> inline auto drawPreview(const T * value, const vec2 & size, F && fun) const
		{
			return drawPreview(typeof<T>(), std::remove_cv_t<ptr_t<void>>(value), size, fun);
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
		~AssetPreview() { this->dispose(); }

		using PreviewMap = typename HashMap<ptr_t<void>, const_ptr_t<Texture>>;
		using TextureList = typename List<ptr_t<Texture>>;

		mutable PreviewMap	m_previewMap;
		mutable TextureList m_textureList;

		template <class ... Args> inline ptr_t<Texture> loadTemp(Args && ... args) const
		{
			m_textureList.push_back(new Texture { std::forward<Args>(args)... });
			return m_textureList.back();
		}

		inline const_ptr_t<Texture> insertPreview(ptr_t<void> value, const_ptr_t<Texture> preview) const
		{
			return m_previewMap.insert({ value, preview }).first->second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_PREVIEW_HPP_