#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <ML/Graphics/Glyph.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Font final
		: public Newable
		, public Disposable
		, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Page		= typename HashMap<uint32_t, Glyph>;
		using PageTable = typename HashMap<uint32_t, Page>;

		struct Info final
		{
			String filename;
			String family;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font();
		explicit Font(const String & filename);
		~Font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		

		const Glyph & getGlyph(uint32_t c, uint32_t characterSize) const;

		Page & getPage(uint32_t characterSize) const;
		
		Glyph loadGlyph(uint32_t c, uint32_t characterSize) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto info() const -> const Info & { return m_info; };

		inline auto pages() const -> const PageTable & { return m_pages; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable PageTable m_pages;
		
		Info	m_info;
		ptr_t<void>	m_library;
		ptr_t<void>	m_face;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, const Font::Info & value)
	{
		return out << value.family;
	}

	inline ML_SERIALIZE(std::ostream & out, const Font & value)
	{
		return out << value.info();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_