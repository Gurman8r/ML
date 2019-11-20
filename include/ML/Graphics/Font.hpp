#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <ML/Graphics/Glyph.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Font final : public Trackable, public NonCopyable
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
		explicit Font(String const & filename);
		~Font() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool loadFromFile(String const & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Glyph const & getGlyph(uint32_t c, uint32_t characterSize) const;

		Page & getPage(uint32_t characterSize) const;
		
		Glyph loadGlyph(uint32_t c, uint32_t characterSize) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto info() const -> Info const & { return m_info; };

		inline auto pages() const -> PageTable const & { return m_pages; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable PageTable m_pages;
		
		Info		m_info;
		void *	m_library;
		void *	m_face;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(std::ostream & out, Font::Info const & value)
	{
		return out << value.family;
	}

	inline ML_SERIALIZE(std::ostream & out, Font const & value)
	{
		return out << value.info();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_