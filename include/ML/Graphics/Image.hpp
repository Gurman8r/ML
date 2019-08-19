#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Image final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Pixels = typename List<uint8_t>;
		using iterator = typename Pixels::iterator;
		using const_iterator = typename Pixels::const_iterator;
		using reverse_iterator = typename Pixels::reverse_iterator;
		using const_reverse_iterator = typename Pixels::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image();
		Image(uint32_t width, uint32_t height);
		Image(uint32_t width, uint32_t height, const Pixels & pixels);
		Image(uint32_t width, uint32_t height, const Pixels & pixels, int32_t channels);
		Image(const Image & copy);
		Image(Image && copy);
		explicit Image(const String & filename);
		explicit Image(const String & filename, bool flipV);
		~Image();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & filename, bool flipV);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & create(uint32_t width, uint32_t height, const vec4b & color);
		Image & create(uint32_t width, uint32_t height, const uint8_t * pixels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Image & flipHorizontally();
		Image & flipVertically();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		vec4b	getPixel(uint32_t x, uint32_t y) const;
		Image & setPixel(uint32_t x, uint32_t y, const vec4b & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto data()		const -> const uint8_t *	{ return &pixels()[0]; }
		inline auto bounds()	const -> const UintRect		{ return UintRect { { 0, 0 }, size() }; }
		inline auto channels()	const -> int32_t			{ return m_channels; }
		inline auto height()	const -> uint32_t			{ return m_size[1]; }
		inline auto pixels()	const -> const Pixels &		{ return m_pixels; }
		inline auto size()		const -> const vec2u &		{ return m_size; }
		inline auto width()		const -> uint32_t			{ return m_size[0]; }

		inline operator bool() const { return !m_pixels.empty(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator					{ return m_pixels.begin(); }
		inline auto begin()		const	-> const_iterator			{ return m_pixels.begin(); }
		inline auto cbegin()	const	-> const_iterator			{ return m_pixels.cbegin(); }
		inline auto end()				-> iterator					{ return m_pixels.end(); }
		inline auto end()		const	-> const_iterator			{ return m_pixels.end(); }
		inline auto cend()		const	-> const_iterator			{ return m_pixels.cend(); }

		inline auto rbegin()			-> reverse_iterator			{ return m_pixels.rbegin(); }
		inline auto rbegin()	const	-> const_reverse_iterator	{ return m_pixels.rbegin(); }
		inline auto crbegin()	const	-> const_reverse_iterator	{ return m_pixels.crbegin(); }
		inline auto rend()				-> reverse_iterator			{ return m_pixels.rend(); }
		inline auto rend()		const	-> const_reverse_iterator	{ return m_pixels.rend(); }
		inline auto crend()		const	-> const_reverse_iterator	{ return m_pixels.crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2u	m_size;
		Pixels	m_pixels;
		int32_t	m_channels;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMAGE_HPP_