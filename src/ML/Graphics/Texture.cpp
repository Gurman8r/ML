#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_TEX_DEFAULT_TARGET	GL::Texture2D
#define ML_TEX_DEFAULT_SMOOTH	true
#define ML_TEX_DEFAULT_REPEAT	false
#define ML_TEX_DEFAULT_MIPMAP	false
#define ML_TEX_DEFAULT_FORMAT	GL::RGBA
#define ML_TEX_DEFAULT_LEVEL	0
#define ML_TEX_DEFAULT_TYPE		GL::UnsignedByte

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture::Texture() : Texture(
		ML_TEX_DEFAULT_TARGET
	)
	{
	}

	Texture::Texture(GL::Target target) : Texture(
		target, 
		ML_TEX_DEFAULT_SMOOTH,
		ML_TEX_DEFAULT_REPEAT
	) 
	{
	}

	Texture::Texture(bool smooth, bool repeated) : Texture(
		ML_TEX_DEFAULT_TARGET,
		smooth, 
		repeated
	)
	{
	}

	Texture::Texture(GL::Format format, bool smooth, bool repeated) : Texture(
		ML_TEX_DEFAULT_TARGET,
		format,
		smooth,
		repeated
	)
	{
	}

	Texture::Texture(GL::Target target, bool smooth, bool repeated) : Texture(
		target, 
		ML_TEX_DEFAULT_FORMAT,
		smooth, 
		repeated
	)
	{
	}

	Texture::Texture(GL::Target target, GL::Format format, bool smooth, bool repeated) : Texture(
		target, 
		format, 
		smooth, 
		repeated, 
		ML_TEX_DEFAULT_MIPMAP
	)
	{
	}

	Texture::Texture(GL::Target target, GL::Format format, bool smooth, bool repeated, bool mipmapped) : Texture(
		target, 
		format, // internal format
		format, // color format
		smooth, 
		repeated, 
		mipmapped
	)
	{
	}

	Texture::Texture(GL::Target target, GL::Format internalFormat, GL::Format colFormat, bool smooth, bool repeated) : Texture(
		target, 
		internalFormat, 
		colFormat, 
		smooth, 
		repeated, 
		ML_TEX_DEFAULT_MIPMAP
	)
	{
	}

	Texture::Texture(GL::Target target, GL::Format internalFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped) : Texture(
		target, 
		internalFormat, 
		colFormat, 
		smooth, 
		repeated, 
		mipmapped, 
		ML_TEX_DEFAULT_LEVEL,
		ML_TEX_DEFAULT_TYPE
	)
	{
	}

	Texture::Texture(GL::Target target, GL::Format internalFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type)
		: IHandle			(NULL)
		, m_size			(vec2u::Zero)
		, m_realSize		(vec2u::Zero)
		, m_target			(target)
		, m_internalFormat	(internalFormat)
		, m_colorFormat		(colFormat)
		, m_smooth			(smooth)
		, m_repeated		(repeated)
		, m_mipmapped		(mipmapped)
		, m_level			(level)
		, m_type			(type)
	{
	}

	Texture::Texture(const Texture & copy) : Texture(
		copy.m_target,
		copy.m_internalFormat,
		copy.m_colorFormat,
		copy.m_smooth,
		copy.m_repeated,
		copy.m_mipmapped,
		copy.m_level,
		copy.m_type
	)
	{
		create(copy);
	}

	Texture::~Texture()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::dispose()
	{
		Texture::bind(NULL);
		if ((*this))
		{
			ML_GL.deleteTextures(1, (*this));
			get_reference() = NULL;
		}
		return !(*this);
	}

	bool Texture::loadFromFile(const String & filename)
	{
		Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return create(value.size()) && update(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::create(const Texture & other)
	{
		return ((other)
			? ((create(other.size()))
				? (update(other))
				: (Debug::logError("Failed to copy texture, failed to create new texture"))
			)
			: (false)
		);
	}

	bool Texture::create(const vec2u & size)
	{
		return create(NULL, size);
	}

	bool Texture::create(const Image & image, const vec2u & size)
	{
		return create(&image.pixels()[0], size);
	}

	bool Texture::create(const Image & image, uint32_t w, uint32_t h)
	{
		return create(&image.pixels()[0], w, h);
	}

	bool Texture::create(const uint8_t * pixels, const vec2u & size)
	{
		return create(pixels, size[0], size[1]);
	}

	bool Texture::create(const uint8_t * pixels, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if (dispose() && set_handle(ML_GL.genTextures(1)))
			{
				m_size = { w, h };
				m_realSize =
				{
					ML_GL.getValidTextureSize(m_size[0]),
					ML_GL.getValidTextureSize(m_size[1])
				};

				static const uint32_t maxSize = ML_GL.getMaxTextureSize();
				if ((m_realSize[0] > maxSize) || (m_realSize[1] > maxSize))
				{
					return Debug::logError(
						"Failed creating texture, size is too large {0} max is {1}",
						m_realSize, 
						vec2u(maxSize)
					);
				}

				Texture::bind(this);
				{
					ML_GL.texImage2D(
						m_target,
						m_level,
						m_internalFormat,
						m_size[0],
						m_size[1],
						0, // border: "This value must be 0."
						m_colorFormat,
						m_type,
						pixels
					);
				}
				Texture::bind(NULL);

				ML_GL.flush();

				setRepeated(m_repeated);

				setSmooth(m_smooth);

				return true;
			}
			else
			{
				return Debug::logError("Failed creating texture, failed setting handle.");
			}
		}
		else
		{
			return Debug::logError("Failed creating texture, invalid size: {0}", vec2u(w, h));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::update(const Texture & other)
	{
		return update(other.copyToImage());
	}

	bool Texture::update(const Texture & other, const UintRect & area)
	{
		return update(other.copyToImage(), area);
	}

	bool Texture::update(const Texture & other, const vec2u & position, const vec2u & size)
	{
		return update(other.copyToImage(), position, size);
	}

	bool Texture::update(const Texture & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(other.copyToImage(), x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(const Image & image)
	{
		return update(&image.pixels()[0], image.bounds());
	}

	bool Texture::update(const Image & image, const UintRect & area)
	{
		return update(&image.pixels()[0], area.position(), area.size());
	}

	bool Texture::update(const Image & image, const vec2u & position, const vec2u & size)
	{
		return update(&image.pixels()[0], position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(const Image & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(&image.pixels()[0], x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(const uint8_t * pixels)
	{
		return update(pixels, width(), height());
	}

	bool Texture::update(const uint8_t * pixels, const UintRect & area)
	{
		return update(pixels, area.position(), area.size());
	}

	bool Texture::update(const uint8_t * pixels, const vec2u & position, const vec2u & size)
	{
		return update(pixels, position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(const uint8_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if ((*this) && (pixels))
			{
				Texture::bind(this);
				{
					ML_GL.texSubImage2D(
						m_target,
						m_level,
						x,
						y,
						w,
						h,
						m_internalFormat,
						m_type,
						pixels
					);
				}
				Texture::bind(NULL);

				ML_GL.flush();

				setRepeated(m_repeated);

				setSmooth(m_smooth);

				return true;
			}
			else
			{
				return Debug::logError("Failed updating texture, failed updating handle.");
			}
		}
		else
		{
			return Debug::logError("Failed updating texture, invalid size: {0}", vec2u(w, h));
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::setMipmapped(bool value)
	{
		if ((*this))
		{
			if ((m_mipmapped = value) && !ML_GL.framebuffersAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture Mipmap Framebuffers Unavailable");
					warned = true;
				}
				m_mipmapped = false;
				return setSmooth(m_smooth);
			}

			Texture::bind(this);
			{
				ML_GL.generateMipmap(m_target);

				ML_GL.texParameter(
					m_target,
					GL::TexMinFilter,
					(((m_smooth)
						? (GL::LinearMipmapLinear)
						: (GL::NearestMipmapNearest))
					)
				);

				ML_GL.texParameter(
					m_target,
					GL::TexMagFilter,
					(((m_smooth)
						? (GL::LinearMipmapLinear)
						: (GL::NearestMipmapNearest))
					)
				);
			}
			Texture::bind(NULL);

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setRepeated(bool value)
	{
		if ((*this))
		{
			if (((m_repeated) = value) && !(ML_GL.edgeClampAvailable()))
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning(
						"OpenGL extension texture_edge_clamp unavailable\n"
						"Artifacts may occur along texture edges");
					warned = true;
				}
			}

			Texture::bind(this);
			{
				ML_GL.texParameter(
					m_target,
					GL::TexWrapS,
					((m_repeated)
						? (GL::Repeat)
						: ((ML_GL.edgeClampAvailable())
							? (GL::ClampToEdge)
							: (GL::Clamp)
						)
					)
				);

				ML_GL.texParameter(
					m_target,
					GL::TexWrapT,
					((m_repeated)
						? (GL::Repeat)
						: ((ML_GL.edgeClampAvailable())
							? (GL::ClampToEdge)
							: (GL::Clamp)
						)
					)
				);
			}
			Texture::bind(NULL);

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if ((*this))
		{
			m_smooth = value;

			Texture::bind(this);
			{
				ML_GL.texParameter(
					m_target,
					GL::TexMinFilter,
					((m_smooth)
						? (GL::Linear)
						: (GL::Nearest)
					)
				);

				ML_GL.texParameter(
					m_target,
					GL::TexMagFilter,
					((m_smooth)
						? GL::Linear
						: (GL::Nearest)
					)
				);
			}
			Texture::bind(NULL);

			ML_GL.flush();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::swap(Texture & other)
	{
		std::swap(get_reference(),	other.get_reference());
		std::swap(m_target,			other.m_target);
		std::swap(m_level,			other.m_level);
		std::swap(m_size,			other.m_size);
		std::swap(m_realSize,		other.m_realSize);
		std::swap(m_internalFormat,	other.m_internalFormat);
		std::swap(m_colorFormat,	other.m_colorFormat);
		std::swap(m_type,			other.m_type);
		std::swap(m_smooth,			other.m_smooth);
		std::swap(m_repeated,		other.m_repeated);
		std::swap(m_mipmapped,		other.m_mipmapped);

		return other;
	}
	
	Texture & Texture::operator=(const Texture & value)
	{
		static Texture temp;
		return swap(temp);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const Image Texture::copyToImage() const
	{
		if ((*this))
		{
			Image::Pixels pixels(width() * height() * 4);

			if ((m_size == m_realSize))
			{
				Texture::bind(this);

				ML_GL.getTexImage(
					m_target,
					0,
					m_internalFormat,
					GL::UnsignedByte,
					&pixels[0]
				);

				Texture::bind(NULL);
			}

			return Image().create(width(), height(), &pixels[0]);
		}
		return Image();
	}

	void Texture::bind(const Texture * value)
	{
		return ((value)
			? (ML_GL.bindTexture(value->target(), (*value)))
			: (ML_GL.bindTexture(GL::Texture2D, NULL))
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}