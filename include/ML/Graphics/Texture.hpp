#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <ML/Graphics/Image.hpp>
#include <ML/Core/Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Texture final : public Trackable, public Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture();
		explicit Texture(GL::Sampler sampler);
		explicit Texture(bool smooth, bool repeated);
		explicit Texture(GL::Format format, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped);
		explicit Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type);
		explicit Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated, bool mipmapped);
		explicit Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type);
		explicit Texture(Texture const & copy);
		explicit Texture(Image const & image);
		~Texture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool loadFromFile(String const & filename);
		bool loadFromImage(Image const & value);
		bool loadFromFaces(const Array<Image const *, 6> & faces);
		bool loadFromFaces(const Array<Image, 6> & faces);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(Texture const & copy);
		bool create(vec2u const & size);
		bool create(Image const & image, vec2u const & size);
		bool create(Image const & image, uint32_t w, uint32_t h);
		bool create(byte_t const * pixels, vec2u const & size);
		bool create(byte_t const * pixels, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(Texture const & other);
		bool update(Texture const & other, UintRect const & area);
		bool update(Texture const & other, vec2u const & position, vec2u const & size);
		bool update(Texture const & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		bool update(Image const & image);
		bool update(Image const & image, UintRect const & area);
		bool update(Image const & image, vec2u const & position, vec2u const & size);
		bool update(Image const & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		
		bool update(byte_t const * pixels);
		bool update(byte_t const * pixels, UintRect const & area);
		bool update(byte_t const * pixels, vec2u const & position, vec2u const & size);
		bool update(byte_t const * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & setMipmapped(bool value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & swap(Texture & value);
		Texture & operator=(Texture const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image copyToImage() const;

		uint32_t getChannels() const;

		Texture const & bind() const;

		Texture const & unbind() const;

		static void bind(Texture const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto sampler()		const -> GL::Sampler	{ return m_sampler; }
		inline auto level()			const -> int32_t		{ return m_level; }
		inline auto internal_fmt()	const -> GL::Format		{ return m_iFormat; }
		inline auto color_fmt()		const -> GL::Format		{ return m_cFormat; }
		inline auto pixel_type()	const -> GL::Type		{ return m_pixType; }
		inline auto size()			const -> vec2u const &	{ return m_size; }
		inline auto real_size()		const -> vec2u const &	{ return m_realSize; }
		inline auto smooth()		const -> bool			{ return m_smooth; }
		inline auto repeated()		const -> bool			{ return m_repeated; }
		inline auto mipmapped()		const -> bool			{ return m_mipmapped; }
		inline auto width()			const -> uint32_t		{ return m_size[0]; }
		inline auto height()		const -> uint32_t		{ return m_size[1]; }
		inline auto real_width()	const -> uint32_t		{ return m_realSize[0]; }
		inline auto real_height()	const -> uint32_t		{ return m_realSize[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::Sampler	m_sampler;		// Sampler Type
		int32_t		m_level;		// Texture Level
		GL::Format	m_iFormat;		// Internal Format
		GL::Format	m_cFormat;		// Color Format
		GL::Type	m_pixType;		// Storage Type
		vec2u		m_size;			// Texture Size
		vec2u		m_realSize;		// Real Texture Size
		bool		m_smooth;		// Is Smooth
		bool		m_repeated;		// Is Repeated
		bool		m_mipmapped;	// Is Mipmapped

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_