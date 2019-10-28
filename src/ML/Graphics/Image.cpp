#include <ML/Graphics/Image.hpp>
#include <ML/Core/Debug.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static inline Image generate_default_image()
	{
		Image img { vec2u{ 512, 512 }, 4 };
		for (uint32_t y = 0; y < img.height(); y++)
		{
			for (uint32_t x = 0; x < img.width(); x++)
			{
				img.setPixel(x, y,
					(((y < img.height() / 2) && (x < img.width() / 2)) ||
					((y >= img.height() / 2) && (x >= img.width() / 2))
						? Color(Color(0.1f).rgb(), 1.0)
						: (((y >= img.height() / 2) || (x >= img.width() / 2))
							? Colors::magenta
							: Colors::green
							)));
			}
		}
		return img;
	}

	const Image Image::Default { generate_default_image() };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image::Image()
		: Image { vec2u { NULL } }
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

	Image::~Image() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Image::dispose()
	{
		if (!m_pixels.empty())
		{
			Pixels().swap(m_pixels);
		}
		m_size = { 0, 0 };
		m_channels = 0;
		return !(*this);
	}
	
	bool Image::loadFromFile(const String & filename)
	{
		return loadFromFile(filename, true);
	}

	bool Image::loadFromFile(const String & filename, bool flip_v)
	{
		return loadFromFile(filename, flip_v, 0);
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
			update({ data, data + capacity() });
			stbi_image_free(data);
			return (*this);
		}
		return dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::update(const vec2u & size, const Color32 & color)
	{
		return update(size, channels(), color);
	}

	Image & Image::update(const Color32 & color)
	{
		return update(size(), channels(), color);
	}
	
	Image & Image::update(const vec2u & size, uint32_t channels, const Color32 & color)
	{
		if (size[0] && size[1] && channels)
		{
			m_size = size;
			m_channels = channels;
			m_pixels.resize(capacity());

			iterator it { begin() };
			while (it != end())
			{
				if (m_channels >= 1) *it++ = color[0];
				if (m_channels >= 2) *it++ = color[1];
				if (m_channels >= 3) *it++ = color[2];
				if (m_channels >= 4) *it++ = color[3];
			}
			return (*this);
		}
		dispose();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Image & Image::update(const vec2u & size, const Pixels & pixels)
	{
		return update(size, m_channels, pixels);
	}

	Image & Image::update(const Pixels & pixels)
	{
		return update(m_size, m_channels, pixels);
	}
	
	Image & Image::update(const vec2u & size, uint32_t channels, const Pixels & pixels)
	{
		if (!pixels.empty() && (pixels.size() == (size[0] * size[1] * channels)))
		{
			m_size = size;
			m_channels = channels;
			m_pixels.assign(pixels.begin(), pixels.end());
			return (*this);
		}
		dispose();
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Image & Image::flipHorizontally()
	{
		if (*this)
		{
			const uint32_t cols { width() * channels() };
			for (uint32_t y = 0; y < height(); ++y)
			{
				iterator lhs { begin() + y * cols };
				iterator rhs { begin() + (y + 1) * cols - channels() };
				for (uint32_t x = 0; x < width() / 2; ++x)
				{
					std::swap_ranges(lhs, lhs + channels(), rhs);
					lhs += channels();
					rhs -= channels();
				}
			}
		}
		return (*this);
	}

	Image & Image::flipVertically()
	{
		if (*this)
		{
			const uint32_t cols { width() * channels() };
			iterator top { begin() };
			iterator bot { end() - cols };
			for (uint32_t y = 0; y < height() / 2; ++y)
			{
				std::swap_ranges(top, top + cols, bot);
				top += cols;
				bot -= cols;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Color32 Image::getPixel(uint32_t x, uint32_t y) const
	{
		Color32 temp { Colors::clear };
		const size_t i { (x + y * m_size[0]) * m_channels };
		if (i < capacity())
		{
			const_iterator it { cbegin() + i };
			if (m_channels >= 1) temp[0] = *(it + 0);
			if (m_channels >= 2) temp[1] = *(it + 1);
			if (m_channels >= 3) temp[2] = *(it + 2);
			if (m_channels >= 4) temp[3] = *(it + 3);
		}
		return temp;
	}

	Image & Image::setPixel(uint32_t x, uint32_t y, const Color32 & color)
	{
		const size_t i { (x + y * m_size[0]) * m_channels };
		if (i < capacity())
		{
			iterator it { begin() + i };
			if (m_channels >= 1) *it++ = color[0];
			if (m_channels >= 2) *it++ = color[1];
			if (m_channels >= 3) *it++ = color[2];
			if (m_channels >= 4) *it++ = color[3];
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}