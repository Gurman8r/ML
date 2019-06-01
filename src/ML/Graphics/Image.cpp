#include <ML/Graphics/Image.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Window/Icon.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Image::Image()
		: m_size(vec2u::Zero)
		, m_pixels(Pixels())
		, m_channels(0)
	{
	}

	Image::Image(uint32_t width, uint32_t height, const byte_t * pixels)
		: m_size(width, height)
		, m_pixels(Pixels())
		, m_channels(0)
	{
		create(width, height, pixels);
	}

	Image::Image(const Image & copy)
		: m_size(copy.m_size)
		, m_pixels(copy.m_pixels)
		, m_channels(copy.m_channels)
	{
	}

	Image::~Image()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Image::dispose()
	{
		m_pixels.clear();
		return true;
	}
	
	bool Image::loadFromFile(const String & filename)
	{
		stbi_set_flip_vertically_on_load(true);
		byte_t * data = stbi_load(
			filename.c_str(),
			&(int32_t &)(m_size[0]),
			&(int32_t &)(m_size[1]),
			&m_channels,
			NULL
		);
		if (data)
		{
			m_pixels.assign(&data[0], &data[m_size[0] * m_size[1] * m_channels]);
		}
		else
		{
			m_size = vec2i::Zero;
			m_channels = 0;
			m_pixels.clear();
		}
		stbi_image_free(data);
		return !m_pixels.empty();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::create(uint32_t width, uint32_t height, const vec4b & color)
	{
		if (width && height)
		{
			// Create a new pixel buffer first for exception safety's sake
			Pixels newPixels(width * height * 4);

			// Fill it with the specified color
			byte_t * ptr = &newPixels[0];
			byte_t * end = ptr + newPixels.size();
			while (ptr < end)
			{
				*(ptr++) = color[0];
				*(ptr++) = color[1];
				*(ptr++) = color[2];
				*(ptr++) = color[3];
			}

			// Commit the new pixel buffer
			m_pixels.swap(newPixels);

			// Assign the new size
			m_size = { width, height };
		}
		else
		{
			// Dump the pixel buffer
			Pixels().swap(m_pixels);

			// Assign the new size
			m_size = 0;
		}
		return (*this);
	}
	
	Image & Image::create(uint32_t width, uint32_t height, const byte_t * pixels)
	{
		if (pixels && width && height)
		{
			// Create a new pixel buffer first for exception safety's sake
			Pixels newPixels(pixels, pixels + width * height * 4);

			// Commit the new pixel buffer
			m_pixels.swap(newPixels);

			// Assign the new size
			m_size = { width, height };
		}
		else
		{
			// Dump the pixel buffer
			Pixels().swap(m_pixels);

			// Assign the new size
			m_size = 0;
		}
		return (*this);
	}
	
	Image & Image::createMaskFromColor(const vec4b & color, byte_t alpha)
	{
		// Make sure that the image is not empty
		if (!m_pixels.empty())
		{
			// Replace the alpha of the pixels that match the transparent color
			byte_t * ptr = &m_pixels[0];
			byte_t * end = ptr + m_pixels.size();
			while (ptr < end)
			{
				if ((ptr[0] == color[0]) &&
					(ptr[1] == color[1]) &&
					(ptr[2] == color[2]) &&
					(ptr[3] == color[3]))
				{
					ptr[3] = alpha;
				}

				ptr += 4;
			}
		}
		return (*this);
	}

	Image & Image::flipHorizontally()
	{
		if ((*this))
		{
			uint32_t rowSize = m_size[0] * 4;

			for (uint32_t y = 0; y < m_size[1]; ++y)
			{
				Pixels::iterator left = m_pixels.begin() + y * rowSize;
				Pixels::iterator right = m_pixels.begin() + (y + 1) * rowSize - 4;

				for (uint32_t x = 0; x < m_size[0] / 2; ++x)
				{
					std::swap_ranges(left, left + 4, right);
					left += 4;
					right -= 4;
				}
			}
		}
		return (*this);
	}

	Image & Image::flipVertically()
	{
		if ((*this))
		{
			uint32_t rowSize = m_size[0] * 4;

			Pixels::iterator top = m_pixels.begin();
			Pixels::iterator bottom = m_pixels.end() - rowSize;

			for (uint32_t y = 0; y < m_size[1] / 2; ++y)
			{
				std::swap_ranges(top, top + rowSize, bottom);
				top += rowSize;
				bottom -= rowSize;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	vec4b	Image::getPixel(uint32_t x, uint32_t y) const
	{
		const byte_t * pixel = &m_pixels[(x + y * m_size[0]) * 4];

		return vec4u(pixel[0], pixel[1], pixel[2], pixel[3]);
	}

	Image & Image::setPixel(uint32_t x, uint32_t y, const vec4b & color)
	{
		byte_t * ptr = &m_pixels[(x + y * m_size[0]) * 4];
		*(ptr++) = color[0];
		*(ptr++) = color[1];
		*(ptr++) = color[2];
		*(ptr++) = color[3];

		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}