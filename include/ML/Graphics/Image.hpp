#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Image final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Pixels					= typename List<uint8_t>;
		using iterator					= typename Pixels::iterator;
		using const_iterator			= typename Pixels::const_iterator;
		using reverse_iterator			= typename Pixels::reverse_iterator;
		using const_reverse_iterator	= typename Pixels::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image();
		Image(const vec2u & size);
		Image(const vec2u & size, uint32_t channels);
		Image(const vec2u & size, const Pixels & pixels);
		Image(const vec2u & size, const Pixels & pixels, uint32_t channels);
		explicit Image(const String & filename);
		explicit Image(const String & filename, bool flip_v);
		explicit Image(const String & filename, bool flip_v, uint32_t req_comp);
		Image(const Image & copy);
		Image(Image && copy);
		~Image();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & filename, bool flip_v);
		bool loadFromFile(const String & filename, bool flip_v, uint32_t req_comp);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & update(const vec2u & size, const vec4b & color);
		Image & update(const vec4b & color);
		Image & update(const vec2u & size, uint32_t channels, const vec4b & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & update(const vec2u & size, const Pixels & pixels);
		Image & update(const Pixels & pixels);
		Image & update(const vec2u & size, uint32_t channels, const Pixels & pixels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Image & flipHorizontally();
		Image & flipVertically();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto bounds()	const -> UintRect			{ return UintRect { { 0, 0 }, size() }; }
		inline auto capacity()	const -> uint32_t			{ return width() * height() * channels(); }
		inline auto channels()	const -> const uint32_t	&	{ return m_channels; }
		inline auto data()		const -> const uint8_t *	{ return m_pixels.data(); }
		inline auto empty()		const -> bool				{ return m_pixels.empty(); }
		inline auto height()	const -> const uint32_t &	{ return m_size[1]; }
		inline auto pixels()	const -> const Pixels &		{ return m_pixels; }
		inline auto size()		const -> const vec2u &		{ return m_size; }
		inline auto width()		const -> const uint32_t &	{ return m_size[0]; }

		inline auto getFormat() const -> GL::Format
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

		inline const uint8_t & operator[](size_t i) const
		{
			return m_pixels[i];
		}

		inline uint8_t & operator[](size_t i)
		{
			return m_pixels[i];
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator					{ return m_pixels.begin(); }
		inline auto begin()		const	-> const_iterator			{ return m_pixels.begin(); }
		inline auto cbegin()	const	-> const_iterator			{ return m_pixels.cbegin(); }
		inline auto cend()		const	-> const_iterator			{ return m_pixels.cend(); }
		inline auto crbegin()	const	-> const_reverse_iterator	{ return m_pixels.crbegin(); }
		inline auto crend()		const	-> const_reverse_iterator	{ return m_pixels.crend(); }
		inline auto end()				-> iterator					{ return m_pixels.end(); }
		inline auto end()		const	-> const_iterator			{ return m_pixels.end(); }
		inline auto rbegin()			-> reverse_iterator			{ return m_pixels.rbegin(); }
		inline auto rbegin()	const	-> const_reverse_iterator	{ return m_pixels.rbegin(); }
		inline auto rend()				-> reverse_iterator			{ return m_pixels.rend(); }
		inline auto rend()		const	-> const_reverse_iterator	{ return m_pixels.rend(); }

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