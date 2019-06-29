#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/Image.hpp>
#include <ML/Core/I_Handle.hpp>

// Default texture parameters
#define ML_TEX_DEFAULT_TARGET	GL::Texture2D
#define ML_TEX_DEFAULT_SMOOTH	true
#define ML_TEX_DEFAULT_REPEAT	false
#define ML_TEX_DEFAULT_MIPMAP	false
#define ML_TEX_DEFAULT_FORMAT	GL::RGBA
#define ML_TEX_DEFAULT_LEVEL	0
#define ML_TEX_DEFAULT_TYPE		GL::UnsignedByte

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Texture final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture();
		explicit Texture(GL::Target target);
		explicit Texture(bool smooth, bool repeated);
		explicit Texture(GL::Format format, bool smooth, bool repeated);
		explicit Texture(GL::Target target, bool smooth, bool repeated);
		explicit Texture(GL::Target target, GL::Format format, bool smooth, bool repeated);
		explicit Texture(GL::Target target, GL::Format format, bool smooth, bool repeated, bool mipmapped);
		explicit Texture(GL::Target target, GL::Format internalFormat, GL::Format colorFormat, bool smooth, bool repeated);
		explicit Texture(GL::Target target, GL::Format internalFormat, GL::Format colorFormat, bool smooth, bool repeated, bool mipmapped);
		explicit Texture(GL::Target target, GL::Format internalFormat, GL::Format colorFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type);
		explicit Texture(const Texture & copy);
		~Texture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromImage(const Image & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(const Texture & copy);
		bool create(const vec2u & size);
		bool create(const Image & image, const vec2u & size);
		bool create(const Image & image, uint32_t w, uint32_t h);
		bool create(const uint8_t * pixels, const vec2u & size);
		bool create(const uint8_t * pixels, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(const Texture & other);
		bool update(const Texture & other, const UintRect & area);
		bool update(const Texture & other, const vec2u & position, const vec2u & size);
		bool update(const Texture & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		bool update(const Image & image);
		bool update(const Image & image, const UintRect & area);
		bool update(const Image & image, const vec2u & position, const vec2u & size);
		bool update(const Image & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		
		bool update(const uint8_t * pixels);
		bool update(const uint8_t * pixels, const UintRect & area);
		bool update(const uint8_t * pixels, const vec2u & position, const vec2u & size);
		bool update(const uint8_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & setMipmapped(bool value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		Texture & setTarget(GL::Target value);
		Texture & setLevel(int32_t value);
		Texture & setInternalFormat(GL::Format value);
		Texture & setColorFormat(GL::Format value);
		Texture & setType(GL::Type value);
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & swap(Texture & value);
		Texture & operator=(const Texture & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Image copyToImage() const;
		static void	bind(const Texture * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto target()		const -> GL::Target		{ return m_target; }
		inline auto level()			const -> int32_t		{ return m_level; }
		inline auto internalFormat()const -> GL::Format		{ return m_internalFormat; }
		inline auto colorFormat()	const -> GL::Format		{ return m_colorFormat; }
		inline auto type()			const -> GL::Type		{ return m_type; }
		inline auto size()			const -> const vec2u &	{ return m_size; }
		inline auto realSize()		const -> const vec2u &	{ return m_realSize; }
		inline auto smooth()		const -> bool			{ return m_smooth; }
		inline auto repeated()		const -> bool			{ return m_repeated; }
		inline auto mipmapped()		const -> bool			{ return m_mipmapped; }
		inline auto width()			const -> uint32_t		{ return m_size[0]; }
		inline auto height()		const -> uint32_t		{ return m_size[1]; }
		inline auto realWidth()		const -> uint32_t		{ return m_realSize[0]; }
		inline auto realHeight()	const -> uint32_t		{ return m_realSize[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::Target	m_target;			// 
		int32_t		m_level;			// 
		GL::Format	m_internalFormat;	// 
		GL::Format	m_colorFormat;		// 
		GL::Type	m_type;				// 
		vec2u		m_size;				// 
		vec2u		m_realSize;			// 
		bool		m_smooth;			// 
		bool		m_repeated;			// 
		bool		m_mipmapped;		// 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_