#ifndef _ML_EDITOR_PREVIEWS_HPP_
#define _ML_EDITOR_PREVIEWS_HPP_

#include <ML/Editor/Editor_Widget.hpp>
#include <ML/Graphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Editor_Previews final : public Editor_Widget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		friend class Editor;

		Editor_Previews();

		~Editor_Previews() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void onEvent(Event const & value) override;

		bool draw() override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using PreviewMap = typename std::unordered_map<void *, Texture const *>;
		using TextureList = typename std::vector<Texture *>;

		mutable PreviewMap	m_previewMap;
		mutable TextureList m_textureList;

		template <class ... Args> inline Texture * loadTemp(Args && ... args) const
		{
			m_textureList.push_back(new Texture{ std::forward<Args>(args)... });
			return m_textureList.back();
		}

		inline Texture const * insertPreview(void * value, Texture const * preview) const
		{
			return m_previewMap.insert({ value, preview }).first->second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		Texture const * getPreview(const typeof<> & type, void * value) const;

		template <class T> inline Texture const * getPreview(void * value) const
		{
			return getPreview(typeof<T>(), value);
		}

		template <class T> inline Texture const * getPreview(T const * value) const
		{
			return getPreview<T>((void *)value);
		}

		template <class T> inline Texture const * getPreview(T const & value) const
		{
			return getPreview<T>(&value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Clbk = std::function<void()>;

		void drawPreview(const typeof<> & type, void * value, vec2 const & size, Clbk fun) const;

		template <
			class T, class F
		> inline void drawPreview(T const * value, vec2 const & size, F && fun) const
		{
			return drawPreview(typeof<T>(), (void *)value, size, fun);
		}

		template <
			class T, class F
		> inline void drawPreview(T const & value, vec2 const & size, F && fun) const
		{
			return drawPreview<T>(&value, size, fun);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EDITOR_PREVIEWS_HPP_