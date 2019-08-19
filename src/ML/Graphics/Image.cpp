#include <ML/Graphics/Image.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image::Image()
		: Image { 0, 0 }
	{
	}

	Image::Image(uint32_t width, uint32_t height)
		: Image { width, height, 4 }
	{
	}

	Image::Image(uint32_t width, uint32_t height, int32_t channels)
		: Image { width, height, Pixels(), channels }
	{
	}

	Image::Image(uint32_t width, uint32_t height, const Pixels & pixels)
		: Image { { width, height }, { pixels }, 4 }
	{
	}

	Image::Image(uint32_t width, uint32_t height, const Pixels & pixels, int32_t channels)
		: Image { { width, height }, { pixels }, { channels } }
	{
	}

	Image::Image(const vec2u & size)
		: Image { size, 4 }
	{
	}

	Image::Image(const vec2u & size, int32_t channels) 
		: Image { size, Pixels(), channels }
	{
	}

	Image::Image(const vec2u & size, const Pixels & pixels)
		: Image { size, pixels, 4 }
	{
	}

	Image::Image(const vec2u & size, const Pixels & pixels, int32_t channels)
		: m_size { size }
		, m_pixels { pixels }
		, m_channels { channels }
	{
	}

	Image::Image(const String & filename)
		: Image {}
	{
		this->loadFromFile(filename);
	}

	Image::Image(const String & filename, bool flip_v)
		: Image {}
	{
		this->loadFromFile(filename, flip_v);
	}

	Image::Image(const Image & copy)
		: Image { copy.m_size, copy.m_pixels, copy.m_channels }
	{
	}

	Image::Image(Image && copy)
		: Image {}
	{
		std::swap(m_size, copy.m_size);
		std::swap(m_pixels, copy.m_pixels);
		std::swap(m_channels, copy.m_channels);
	}

	Image::~Image() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	bool Image::loadFromFile(const String & filename, bool flip_v)
	{
		return loadFromFile(filename, flip_v, 0);
	}

	bool Image::loadFromFile(const String & filename, bool flip_v, int32_t channels)
	{
		stbi_set_flip_vertically_on_load(flip_v);

		int32_t w, h, c;
		if (uint8_t * data = stbi_load(filename.c_str(), &w, &h, &c, channels))
		{
			update(w, h, c, { data, data + (w * h * c) });
			stbi_image_free(data);
			return (*this);
		}
		else
		{
			return dispose();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::update(uint32_t width, uint32_t height, int32_t channels, const vec4b & color)
	{
		return update({ width, height }, channels, color);
	}

	Image & Image::update(uint32_t width, uint32_t height, const vec4b & color)
	{
		return update({ width, height }, m_channels, color);
	}

	Image & Image::update(const vec2u & size, const vec4b & color)
	{
		return update(size, m_channels, color);
	}
	
	Image & Image::update(const vec2u & size, int32_t channels, const vec4b & color)
	{
		if (size[0] && size[1])
		{
			return setSize(size).setChannels(channels).setPixels(color);
			
		}
		else
		{
			dispose(); return (*this);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::update(uint32_t width, uint32_t height, int32_t channels, const Pixels & pixels)
	{
		return update({ width, height }, channels, pixels);
	}
	
	Image & Image::update(uint32_t width, uint32_t height, const Pixels & pixels)
	{
		return update({ width, height }, m_channels, pixels);
	}

	Image & Image::update(const vec2u & size, const Pixels & pixels)
	{
		return update(size, m_channels, pixels);
	}
	
	Image & Image::update(const vec2u & size, int32_t channels, const Pixels & pixels)
	{
		if (pixels && size[0] && size[1] && channels)
		{
			return setSize(size).setChannels(channels).setPixels(pixels);
		}
		else
		{
			dispose(); return (*this);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::setChannels(int32_t value)
	{
		m_channels = value;
		if (capacity() != m_pixels.size())
		{
			m_pixels.resize(capacity());
		}
		return (*this);
	}

	Image & Image::setSize(uint32_t width, uint32_t height)
	{
		return setSize({ width, height });
	}

	Image & Image::setSize(const vec2u & value)
	{
		m_size = value;
		if (capacity() != m_pixels.size())
		{
			m_pixels.resize(capacity());
		}
		return (*this);
	}

	Image & Image::setWidth(uint32_t value)
	{
		return setSize({ value, width() });
	}

	Image & Image::setHeight(uint32_t value)
	{
		return setSize({ width(), value });
	}

	Image & Image::setPixels(const Pixels & value)
	{
		m_pixels = value;
		return (*this);
	}

	Image & Image::setPixels(const vec4b & value)
	{
		iterator it = begin();
		while (it != end())
		{
			*it++ = value[0];
			*it++ = value[1];
			*it++ = value[2];
			*it++ = value[3];
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::flipHorizontally()
	{
		if (*this)
		{
			uint32_t rows = m_size[0] * m_channels;

			for (uint32_t y = 0; y < m_size[1]; ++y)
			{
				iterator left = begin() + y * rows;
				iterator right = begin() + (y + 1) * rows - m_channels;

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

			iterator top = begin();
			iterator bottom = end() - rows;

			for (uint32_t y = 0; y < m_size[1] / 2; ++y)
			{
				std::swap_ranges(top, top + rows, bottom);
				top += rows;
				bottom -= rows;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}