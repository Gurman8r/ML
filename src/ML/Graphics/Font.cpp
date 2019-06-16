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
		: m_pages	()
		, m_library	(nullptr)
		, m_face	(nullptr)
	{
	}

	Font::Font(const Font & copy)
		: m_pages	(copy.m_pages)
		, m_library	(copy.m_library)
		, m_face	(copy.m_face)
	{
	}

	Font::~Font()
	{
		dispose();
	}

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
		FT_Library library;
		if (FT_Init_FreeType(&library) != EXIT_SUCCESS)
		{
			return Debug::logError("Failed to load font \"{0}\" (failed to open FreeType)", 
				filename);
		}
		m_library = library;

		// Load the new fonts face from the specified file
		FT_Face face;
		if (FT_New_Face(static_cast<FT_Library>(m_library), filename.c_str(), 0, &face) != EXIT_SUCCESS)
		{
			return Debug::logError("Failed to load font \"{0}\" (failed to create the font face)",
				filename);
		}

		// Load the stroker that will be used to outline the fonts
		FT_Stroker stroker;
		if (FT_Stroker_New(static_cast<FT_Library>(m_library), &stroker) != EXIT_SUCCESS)
		{
			FT_Done_Face(face);
			return Debug::logError("Failed to load font \"{0}\" (failed to create the stroker)", 
				filename);
		}

		// Select the unicode character map
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != EXIT_SUCCESS)
		{
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			return Debug::logError("Failed to load font \"{0}\" (failed to set the Unicode character set)", 
				filename);
		}

		// Store the loaded fonts in our ugly void * :)
		m_face = face;

		// Store the fonts information
		m_info.family = face->family_name ? face->family_name : String();

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Glyph & Font::getGlyph(uint32_t value, uint32_t size) const
	{
		GlyphTable & table = m_pages[size];

		GlyphTable::const_iterator it;
		if ((it = table.find(value)) != table.end())
		{
			return it->second;
		}
		else
		{
			return table.insert({ value, loadGlyph(value, size) }).first->second;
		}
	}

	const Font::Info & Font::getInfo() const
	{
		return m_info;
	}

	Glyph Font::loadGlyph(uint32_t value, uint32_t size) const
	{
		Glyph glyph;

		FT_Face face;
		if (!(face = static_cast<FT_Face>(m_face)))
		{
			return glyph;
		}

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, size);

		// Disable byte-alignment restriction
		ML_GL.pixelStore(GL::UnpackAlignment, 1);

		// Load character glyph 
		if (FT_Load_Char(face, value, FT_LOAD_RENDER) != EXIT_SUCCESS)
		{
			Debug::logWarning("Failed to load Glyph \'{0}\'", (char)value);
			return glyph;
		}

		glyph.bounds = {
			(float)face->glyph->bitmap_left,
			(float)face->glyph->bitmap_top,
			(float)face->glyph->bitmap.width,
			(float)face->glyph->bitmap.rows
		};

		glyph.advance = (uint32_t)face->glyph->advance.x;

		// Only load a texture for characters requiring a graphic
		if ((value != ' ') && isgraph(value))
		{
			if (!glyph.texture.create(face->glyph->bitmap.buffer, (vec2u)glyph.size()))
			{
				Debug::logWarning("Failed Loading Glyph Texture: \'{0}\'", (char)value);
			}
		}

		return glyph;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}