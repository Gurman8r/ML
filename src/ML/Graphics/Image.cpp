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
		: Image { { uninit } }
	{
	}

	Image::Image(const vec2u & size)
		: Image { size, 4 }
	{
	}

	Image::Image(const vec2u & size, uint32_t channels) 
		: Image { size, Pixels(), channels }
	{
	}

	Image::Image(const vec2u & size, const Pixels & pixels)
		: Image { size, pixels, 4 }
	{
	}

	Image::Image(const vec2u & size, const Pixels & pixels, uint32_t channels)
		: m_size { size }
		, m_pixels { pixels }
		, m_channels { channels }
	{
		if (const uint32_t c { this->capacity() })
		{
			if (m_pixels.empty() || (m_pixels.size() != c))
			{
				m_pixels.resize(c);
			}
		}
	}

	Image::Image(const String & filename)
		: Image { filename, false }
	{
	}

	Image::Image(const String & filename, bool flip_v)
		: Image { filename, flip_v, 0 }
	{
	}

	Image::Image(const String & filename, bool flip_v, uint32_t req_comp)
		: Image {}
	{
		this->loadFromFile(filename, flip_v, req_comp);
	}

	Image::Image(const Image & copy)
		: Image {}
	{
		this->update(copy.m_size, copy.m_channels, copy.m_pixels);
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
		}
		m_size = { 0, 0 };
		m_channels = 0;
		return !(*this);
	}
	
	bool Image::loadFromFile(const String & filename)
	{
		return this->loadFromFile(filename, true);
	}

	bool Image::loadFromFile(const String & filename, bool flip_v)
	{
		return this->loadFromFile(filename, flip_v, 0);
	}

	bool Image::loadFromFile(const String & filename, bool flip_v, uint32_t req_comp)
	{
		stbi_set_flip_vertically_on_load(flip_v);

		if (byte_t * data = stbi_load(
			filename.c_str(), 
			(int32_t *)(&m_size[0]),
			(int32_t *)(&m_size[1]),
			(int32_t *)(&m_channels),
			req_comp
		))
		{
			this->update({ data, data + this->capacity() });
			
			stbi_image_free(data);
			
			return (*this);
		}
		return this->dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::update(const vec2u & size, const vec4b & color)
	{
		return this->update(size, m_channels, color);
	}

	Image & Image::update(const vec4b & color)
	{
		return this->update(m_size, m_channels, color);
	}
	
	Image & Image::update(const vec2u & size, uint32_t channels, const vec4b & color)
	{
		if (size[0] && size[1] && channels)
		{
			m_size = size;
			m_channels = channels;
			m_pixels.resize(capacity());

			iterator it = this->begin();
			while (it != this->end())
			{
				if (m_channels >= 1) *it++ = color[0];
				if (m_channels >= 2) *it++ = color[1];
				if (m_channels >= 3) *it++ = color[2];
				if (m_channels >= 4) *it++ = color[3];
			}
			return (*this);
		}
		this->dispose();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::update(const vec2u & size, const Pixels & pixels)
	{
		return this->update(size, m_channels, pixels);
	}

	Image & Image::update(const Pixels & pixels)
	{
		return this->update(m_size, m_channels, pixels);
	}
	
	Image & Image::update(const vec2u & size, uint32_t channels, const Pixels & pixels)
	{
		if (pixels && (pixels.size() == (size[0] * size[1] * channels)))
		{
			m_size = size;
			m_channels = channels;
			m_pixels = pixels;
			return (*this);
		}
		this->dispose();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::flipHorizontally()
	{
		if (*this)
		{
			uint32_t rows { m_size[0] * m_channels };

			for (uint32_t y = 0; y < m_size[1]; ++y)
			{
				iterator left	{ this->begin() + y * rows };
				iterator right	{ this->begin() + (y + 1) * rows - m_channels };

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
			uint32_t rows	{ m_size[0] * m_channels };
			iterator top	{ this->begin() };
			iterator bottom { this->end() - rows };

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