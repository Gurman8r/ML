#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/Rect.hpp>
#include <ML/Core/List.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Image final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Pixels = typename List<uint8_t>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image();
		explicit Image(const String & filename);
		Image(uint32_t width, uint32_t height, const uint8_t * pixels);
		Image(const Image & copy);
		Image(Image && copy);
		~Image();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;		

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & create(uint32_t width, uint32_t height, const vec4b & color);
		Image & create(uint32_t width, uint32_t height, const uint8_t * pixels);
		Image & createMaskFromColor(const vec4b & color, uint8_t alpha = 0);
		
		Image & flipHorizontally();
		Image & flipVertically();

		vec4b	getPixel(uint32_t x, uint32_t y) const;
		Image & setPixel(uint32_t x, uint32_t y, const vec4b & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto size()		const -> const vec2u &		{ return m_size; }
		inline auto pixels()	const -> const Pixels &		{ return m_pixels; }
		inline auto pixelsPtr() const -> const uint8_t *	{ return &pixels()[0]; }
		inline auto channels()	const -> int32_t			{ return m_channels; }
		inline auto width()		const -> const uint32_t		{ return m_size[0]; }
		inline auto height()	const -> const uint32_t		{ return m_size[1]; }
		inline auto bounds()	const -> const UintRect		{ return UintRect { vec2u { 0, 0 }, size() }; }

		inline operator bool() const { return !m_pixels.empty(); }

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