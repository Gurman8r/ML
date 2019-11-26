#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <ML/Graphics/Color.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Image final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Pixels					= typename std::vector<byte_t>;
		using iterator					= typename Pixels::iterator;
		using const_iterator			= typename Pixels::const_iterator;
		using reverse_iterator			= typename Pixels::reverse_iterator;
		using const_reverse_iterator	= typename Pixels::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static const Image Default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image();
		Image(vec2u const & size);
		Image(vec2u const & size, uint32_t channels);
		Image(vec2u const & size, Pixels const & pixels);
		Image(vec2u const & size, Pixels const & pixels, uint32_t channels);
		explicit Image(String const & filename);
		explicit Image(String const & filename, bool flip_v);
		explicit Image(String const & filename, bool flip_v, uint32_t req_comp);
		Image(Image const & copy);
		~Image() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool loadFromFile(String const & filename);
		bool loadFromFile(String const & filename, bool flip_v);
		bool loadFromFile(String const & filename, bool flip_v, uint32_t req_comp);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & update(vec2u const & size, Color32 const & color);
		Image & update(Color32 const & color);
		Image & update(vec2u const & size, uint32_t channels, Color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & update(vec2u const & size, Pixels const & pixels);
		Image & update(Pixels const & pixels);
		Image & update(vec2u const & size, uint32_t channels, Pixels const & pixels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Image & flipHorizontally();
		Image & flipVertically();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Color32 getPixel(uint32_t x, uint32_t y) const;
		Image & setPixel(uint32_t x, uint32_t y, Color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto bounds()	const -> UintRect			{ return UintRect { { 0, 0 }, size() }; }
		inline auto capacity()	const -> uint32_t			{ return width() * height() * channels(); }
		inline auto channels()	const -> uint32_t const	&	{ return m_channels; }
		inline auto data()		const -> byte_t const *		{ return m_pixels.data(); }
		inline auto empty()		const -> bool				{ return m_pixels.empty(); }
		inline auto height()	const -> uint32_t const &	{ return m_size[1]; }
		inline auto pixels()	const -> Pixels const &		{ return m_pixels; }
		inline auto size()		const -> vec2u const &		{ return m_size; }
		inline auto width()		const -> uint32_t const &	{ return m_size[0]; }

		inline GL::Format getFormat() const
		{
			switch (m_channels)
			{
			case 1 : return GL::Red;
			case 3 : return GL::RGB;
			case 4 :
			default: return GL::RGBA;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const 
		{ 
			return !this->empty();
		}

		inline byte_t const & operator[](size_t i) const
		{
			return m_pixels[i];
		}

		inline byte_t & operator[](size_t i)
		{
			return m_pixels[i];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				{ return m_pixels.begin(); }
		inline auto begin()		const	{ return m_pixels.begin(); }
		inline auto cbegin()	const	{ return m_pixels.cbegin(); }
		inline auto cend()		const	{ return m_pixels.cend(); }
		inline auto crbegin()	const	{ return m_pixels.crbegin(); }
		inline auto crend()		const	{ return m_pixels.crend(); }
		inline auto end()				{ return m_pixels.end(); }
		inline auto end()		const	{ return m_pixels.end(); }
		inline auto rbegin()			{ return m_pixels.rbegin(); }
		inline auto rbegin()	const	{ return m_pixels.rbegin(); }
		inline auto rend()				{ return m_pixels.rend(); }
		inline auto rend()		const	{ return m_pixels.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2u		m_size;
		Pixels		m_pixels;
		uint32_t	m_channels;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMAGE_HPP_