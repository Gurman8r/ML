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

		const_ptr_t<Texture> getPreview(const typeof<> & type, voidptr_t value) const;

		template <class T> inline const_ptr_t<Texture> getPreview(voidptr_t value) const
		{
			return getPreview(typeof<T>(), value);
		}

		template <class T> inline const_ptr_t<Texture> getPreview(const T * value) const
		{
			return getPreview<T>((voidptr_t)value);
		}

		template <class T> inline const_ptr_t<Texture> getPreview(const T & value) const
		{
			return getPreview<T>(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clbk = std::function<void()>;

		void drawPreview(const typeof<> & type, voidptr_t value, const vec2 & size, Clbk fun) const;

		template <
			class T, class F
		> inline void drawPreview(const_ptr_t<T> value, const vec2 & size, F && fun) const
		{
			return drawPreview(typeof<T>(), (voidptr_t)value, size, fun);
		}

		template <
			class T, class F
		> inline void drawPreview(const T & value, const vec2 & size, F && fun) const
		{
			return drawPreview<T>(&value, size, fun);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<AssetPreview>;

		AssetPreview() : m_previewMap {}, m_textureList {} {}
		~AssetPreview() { this->dispose(); }

		using PreviewMap = typename HashMap<voidptr_t, const_ptr_t<Texture>>;
		using TextureList = typename List<ptr_t<Texture>>;

		mutable PreviewMap	m_previewMap;
		mutable TextureList m_textureList;

		template <class ... Args> inline ptr_t<Texture> loadTemp(Args && ... args) const
		{
			m_textureList.push_back(new Texture { std::forward<Args>(args)... });
			return m_textureList.back();
		}

		inline const_ptr_t<Texture> insertPreview(voidptr_t value, const_ptr_t<Texture> preview) const
		{
			return m_previewMap.insert({ value, preview }).first->second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_PREVIEW_HPP_