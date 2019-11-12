#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

# include <ft2build.h>
# include FT_FREETYPE_H
# include FT_GLYPH_H
# include FT_OUTLINE_H
# include FT_BITMAP_H
# include FT_STROKER_H
# ifdef ML_SYSTEM_WINDOWS
#	pragma comment(lib, "freetype.lib")
# endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Font::Font()
		: m_info	{}
		, m_pages	{}
		, m_library	{ nullptr }
		, m_face	{ nullptr }
	{
	}

	Font::Font(const String & filename)
		: Font {}
	{
		loadFromFile(filename);
	}

	Font::~Font() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * */

	bool Font::dispose()
	{
		m_pages.clear();

		// Destroy the fonts face
		if (m_face)
		{
			FT_Done_Face(static_cast<FT_Face>(m_face));
		}

		// Close the library
		if (m_library)
		{
			FT_Done_FreeType(static_cast<FT_Library>(m_library));
		}

		return true;
	}

	bool Font::loadFromFile(const String & filename)
	{
		if (m_library) return false;

		m_info.filename = filename;

		FT_Library library;
		if (FT_Init_FreeType(&library) != EXIT_SUCCESS)
		{
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to open FreeType)", 
				filename
			);
		}
		m_library = library;

		// Load the new fonts face from the specified file
		FT_Face face;
		if (FT_New_Face(static_cast<FT_Library>(m_library), filename.c_str(), 0, &face) != EXIT_SUCCESS)
		{
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to create the font face)",
				filename
			);
		}

		// Load the stroker that will be used to outline the fonts
		FT_Stroker stroker;
		if (FT_Stroker_New(static_cast<FT_Library>(m_library), &stroker) != EXIT_SUCCESS)
		{
			FT_Done_Face(face);
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to create the stroker)", 
				filename
			);
		}

		// Select the unicode character map
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != EXIT_SUCCESS)
		{
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			return Debug::logError(
				"Failed loading font \"{0}\" (failed to set the Unicode character set)", 
				filename
			);
		}

		// Store the loaded fonts in our ugly Own<void> :)
		m_face = face;

		// Store the fonts information
		m_info.family = face->family_name ? face->family_name : String();

		return m_library;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Glyph & Font::getGlyph(uint32_t c, uint32_t characterSize) const
	{
		Page & page { getPage(characterSize) };
		auto it { page.find(c) };
		if (it == page.end())
		{
			it = page.insert({ c, loadGlyph(c, characterSize) }).first;
		}
		return it->second;
	}

	Font::Page & Font::getPage(uint32_t characterSize) const
	{
		auto it { m_pages.find(characterSize) };
		if (it == m_pages.end())
		{
			it = m_pages.insert({ characterSize, Page {} }).first;
		}
		return it->second;
	}

	Glyph Font::loadGlyph(uint32_t c, uint32_t characterSize) const
	{
		Glyph glyph;

		FT_Face face;
		if (!(face = static_cast<FT_Face>(m_face)))
		{
			return glyph;
		}

		// Set size loading glyphs as
		FT_Set_Pixel_Sizes(face, 0, characterSize);

		// Disable byte-alignment restriction
		ML_GL.pixelStore(GL::UnpackAlignment, 1);

		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != EXIT_SUCCESS)
		{
			Debug::logWarning("Failed loading Glyph \'{0}\'", (char)c);
			return glyph;
		}

		glyph.bounds = {
			(float_t)face->glyph->bitmap_left,
			(float_t)face->glyph->bitmap_top,
			(float_t)face->glyph->bitmap.width,
			(float_t)face->glyph->bitmap.rows
		};

		glyph.advance = (uint32_t)face->glyph->advance.x;

		// Only load a texture for characters requiring a graphic
		if ((c != ' ') && std::isgraph(c))
		{
			if (!glyph.texture.create(face->glyph->bitmap.buffer, (vec2u)glyph.size()))
			{
				Debug::logWarning("Failed Loading Glyph Texture: \'{0}\'", (char)c);
			}
		}

		return glyph;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}