#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/Vector2.hpp>
#include <ML/Core/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Image final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
	public:
		using Pixels = typename List<uint8_t>;

	public:
		Image();
		explicit Image(const String & filename);
		Image(uint32_t width, uint32_t height, const uint8_t * pixels);
		Image(const Image & copy);
		Image(Image && copy);
		~Image();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;		

	public:
		Image & create(uint32_t width, uint32_t height, const vec4b & color);
		Image & create(uint32_t width, uint32_t height, const uint8_t * pixels);
		Image & createMaskFromColor(const vec4b & color, uint8_t alpha = 0);
		
		Image & flipHorizontally();
		Image & flipVertically();

		vec4b	getPixel(uint32_t x, uint32_t y) const;
		Image & setPixel(uint32_t x, uint32_t y, const vec4b & color);

	public:
		inline const vec2u &	size()		const { return m_size; }
		inline const Pixels &	pixels()	const { return m_pixels; }
		inline const uint8_t *	pixelsPtr() const { return &pixels()[0]; }
		inline const int32_t	channels()	const { return m_channels; }
		inline const uint32_t	width()		const { return m_size[0]; }
		inline const uint32_t	height()	const { return m_size[1]; }
		inline const UintRect	bounds()	const { return { vec2u::Zero, size() }; }

	public:
		inline operator bool() const { return !m_pixels.empty(); }

	private:
		vec2u	m_size;
		Pixels	m_pixels;
		int32_t	m_channels;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMAGE_HPP_