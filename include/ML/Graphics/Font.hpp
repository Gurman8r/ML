#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <ML/Graphics/Glyph.hpp>
#include <ML/Core/I_Readable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Font final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using GlyphTable = Map<uint32_t, Glyph>;
		using PageTable  = Map<uint32_t, GlyphTable>;

		struct Info final
		{
			String family;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Font();
		Font(const Font & copy);
		~Font();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Glyph & getGlyph(uint32_t value, uint32_t size) const;
		const Info	& getInfo() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mutable PageTable m_pages;
		
		Info	m_info;
		void *	m_library;
		void *	m_face;

		Glyph loadGlyph(uint32_t value, uint32_t size) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline ML_SERIALIZE(Ostream & out, const Font::Info & value)
	{
		return out << value.family;
	}

	inline ML_SERIALIZE(Ostream & out, const Font & value)
	{
		return out << value.getInfo();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_