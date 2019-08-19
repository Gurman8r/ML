#include <ML/Graphics/Image.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image::Image()
		: Image { 0, 0 }
	{
	}

	Image::Image(uint32_t width, uint32_t height)
		: Image { width, height, Pixels {} }
	{
	}

	Image::Image(uint32_t width, uint32_t height, const Pixels & pixels)
		: Image { width, height, pixels, 4 }
	{
	}

	Image::Image(uint32_t width, uint32_t height, const Pixels & pixels, int32_t channels)
		: m_size { width, height }
		, m_pixels { pixels }
		, m_channels { channels }
	{
	}

	Image::Image(const Image & copy)
		: m_size { copy.m_size } 
		, m_pixels { copy.m_pixels }
		, m_channels { copy.m_channels }
	{
	}

	Image::Image(Image && copy)
		: Image {}
	{
		std::swap(m_size, copy.m_size);
		std::swap(m_pixels, copy.m_pixels);
		std::swap(m_channels, copy.m_channels);
	}

	Image::Image(const String & filename)
		: Image {}
	{
		this->loadFromFile(filename);
	}

	Image::Image(const String & filename, bool flipV)
		: Image {}
	{
		this->loadFromFile(filename, flipV);
	}

	Image::~Image() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Image::dispose()
	{
		if (m_pixels)
		{
			Pixels().swap(m_pixels);
			m_size = { 0, 0 };
			m_channels = 0;
		}
		return !m_pixels;
	}
	
	bool Image::loadFromFile(const String & filename)
	{
		return loadFromFile(filename, true);
	}

	bool Image::loadFromFile(const String & filename, bool flipV)
	{
		stbi_set_flip_vertically_on_load(flipV);
		
		if (uint8_t * data = stbi_load(
			filename.c_str(),
			&((int32_t &)m_size[0]),
			&((int32_t &)m_size[1]),
			&m_channels,
			NULL
		))
		{
			m_pixels.assign(&data[0], &data[m_size[0] * m_size[1] * m_channels]);
			stbi_image_free(data);
			return (*this);
		}
		else
		{
			return dispose();
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::create(uint32_t width, uint32_t height, const vec4b & color)
	{
		if (width && height)
		{
			Pixels temp((m_size[0] = width) * (m_size[1] = height) * (m_channels = 4));

			uint8_t * ptr { &temp[0] };
			uint8_t * end { ptr + temp.size() };
			while (ptr < end)
			{
				*ptr++ = color[0];
				*ptr++ = color[1];
				*ptr++ = color[2];
				*ptr++ = color[3];
			}
			m_pixels.swap(temp);
			m_size = { width, height };
		}
		else
		{
			dispose();
		}
		return (*this);
	}
	
	Image & Image::create(uint32_t width, uint32_t height, const uint8_t * pixels)
	{
		if (pixels && width && height)
		{
			m_pixels.assign(
				pixels, 
				pixels + ((m_size[0] = width) * (m_size[1] = height) * (m_channels = 4))
			);
		}
		else
		{
			dispose();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::flipHorizontally()
	{
		if (*this)
		{
			uint32_t rows = m_size[0] * m_channels;

			for (uint32_t y = 0; y < m_size[1]; ++y)
			{
				iterator left = this->begin() + y * rows;
				iterator right = this->begin() + (y + 1) * rows - m_channels;

				for (uint32_t x = 0; x < m_size[0] / 2; ++x)
				{
					std::swap_ranges(left, left + m_channels, right);
					left += m_channels;
					right -= m_channels;
				}
			}
		}
		return (*this);
	}

	Image & Image::flipVertically()
	{
		if (*this)
		{
			uint32_t rows = m_size[0] * m_channels;

			iterator top = this->begin();
			iterator bottom = this->end() - rows;

			for (uint32_t y = 0; y < m_size[1] / 2; ++y)
			{
				std::swap_ranges(top, top + rows, bottom);
				top += rows;
				bottom -= rows;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	vec4b	Image::getPixel(uint32_t x, uint32_t y) const
	{
		const uint8_t * pixel { &m_pixels[(x + y * m_size[0]) * m_channels] };
		return vec4b { pixel[0], pixel[1], pixel[2], pixel[3] };
	}

	Image & Image::setPixel(uint32_t x, uint32_t y, const vec4b & color)
	{
		uint8_t * ptr { &m_pixels[(x + y * m_size[0]) * m_channels] };
		*(ptr++) = color[0];
		*(ptr++) = color[1];
		*(ptr++) = color[2];
		*(ptr++) = color[3];
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}