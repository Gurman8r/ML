#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture::Texture() : Texture(
		ML_TEX_DEFAULT_TARGET
	)
	{
	}

	Texture::Texture(GL::Sampler sampler) : Texture(
		sampler, 
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

	Texture::Texture(GL::Sampler sampler, bool smooth, bool repeated) : Texture(
		sampler, 
		ML_TEX_DEFAULT_FORMAT,
		smooth, 
		repeated
	)
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated) : Texture(
		sampler, 
		format, 
		smooth, 
		repeated, 
		ML_TEX_DEFAULT_MIPMAP
	)
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped) : Texture(
		sampler, 
		format, // internal format
		format, // color format
		smooth, 
		repeated, 
		mipmapped
	)
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format internalFormat, GL::Format colFormat, bool smooth, bool repeated) : Texture(
		sampler, 
		internalFormat, 
		colFormat, 
		smooth, 
		repeated, 
		ML_TEX_DEFAULT_MIPMAP
	)
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format internalFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped) : Texture(
		sampler, 
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

	Texture::Texture(GL::Sampler sampler, GL::Format internalFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type)
		: I_Handle			(NULL)
		, m_size			(vec2u { 0, 0 })
		, m_realSize		(vec2u { 0, 0 })
		, m_sampler			(sampler)
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
		copy.m_sampler,
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

	Texture::~Texture() { this->dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::dispose()
	{
		this->unbind();
		if (*this)
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

	bool Texture::loadFromFaces(const Array<const Image *, 6> & faces)
	{
		// Validate Target
		if (m_sampler != GL::TextureCubeMap)
			return Debug::logError("Load from faces only available for {0}s.",
				GL::TextureCubeMap
			);

		// Validate Images
		for (size_t i = 0; i < faces.size(); i++)
		{
			if (!faces[i])
			{
				return Debug::logError("Face not found: {0}", i);
			}
			else if (faces[i]->pixels().empty())
			{
				return Debug::logError("Face is empty: {0}", i);
			}
			else if (i == 0)
			{
				m_size = m_realSize = { faces[i]->size() };
			}
			else if (faces[i]->size() != m_size)
			{
				Debug::logWarning("Face size mismatch: {0} | {1}", 
					m_size, faces[i]->size()
				);
			}
		}

		// Create Texture
		if (this->dispose() && set_handle(ML_GL.genTextures(1)))
		{
			this->bind();
			for (size_t i = 0; i < faces.size(); i++)
			{
				ML_GL.texImage2D(
					GL::CubeMap_Positive_X + (uint32_t)i,
					m_level,
					m_internalFormat,
					faces[i]->size()[0],
					faces[i]->size()[1],
					0,
					m_colorFormat,
					m_type,
					faces[i]->data()
				);
			}
			this->unbind();
			ML_GL.flush();
			this->setRepeated(m_repeated);
			this->setSmooth(m_smooth);
			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::create(const Texture & other)
	{
		return ((other)
			? (this->create(other.size())
				? this->update(other)
				: Debug::logError("Failed to copy texture, failed to create new texture")) 
			: false
		);
	}

	bool Texture::create(const vec2u & size)
	{
		return this->create(nullptr, size);
	}

	bool Texture::create(const Image & image, const vec2u & size)
	{
		return this->create(image.data(), size);
	}

	bool Texture::create(const Image & image, uint32_t w, uint32_t h)
	{
		return this->create(image.data(), w, h);
	}

	bool Texture::create(const uint8_t * pixels, const vec2u & size)
	{
		return this->create(pixels, size[0], size[1]);
	}

	bool Texture::create(const uint8_t * pixels, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if (this->dispose() && set_handle(ML_GL.genTextures(1)))
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
						vec2u { maxSize , maxSize }
					);
				}

				this->bind();
				{
					ML_GL.texImage2D(
						m_sampler,
						m_level,
						m_internalFormat,
						m_size[0],
						m_size[1],
						0, // border: "This value must be 0" -khronos.org
						m_colorFormat,
						m_type,
						pixels
					);
				}
				this->unbind();

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
			return Debug::logError("Failed creating texture, invalid size: {0}", vec2u { w, h });
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::update(const Texture & other)
	{
		return this->update(other.copyToImage());
	}

	bool Texture::update(const Texture & other, const UintRect & area)
	{
		return this->update(other.copyToImage(), area);
	}

	bool Texture::update(const Texture & other, const vec2u & position, const vec2u & size)
	{
		return this->update(other.copyToImage(), position, size);
	}

	bool Texture::update(const Texture & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return this->update(other.copyToImage(), x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(const Image & image)
	{
		return this->update(image.data(), image.bounds());
	}

	bool Texture::update(const Image & image, const UintRect & area)
	{
		return this->update(image.data(), area.position(), area.size());
	}

	bool Texture::update(const Image & image, const vec2u & position, const vec2u & size)
	{
		return this->update(image.data(), position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(const Image & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return this->update(image.data(), x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(const uint8_t * pixels)
	{
		return this->update(pixels, UintRect { width(), height() });
	}

	bool Texture::update(const uint8_t * pixels, const UintRect & area)
	{
		return this->update(pixels, area.position(), area.size());
	}

	bool Texture::update(const uint8_t * pixels, const vec2u & position, const vec2u & size)
	{
		return this->update(pixels, position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(const uint8_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if ((*this) && (pixels))
			{
				this->bind();
				{
					ML_GL.texSubImage2D(
						m_sampler,
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
				this->unbind();

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
			return Debug::logError("Failed updating texture, invalid size: {0}", vec2u { w, h });
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::setMipmapped(bool value)
	{
		if (*this)
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

			this->bind();

			ML_GL.generateMipmap(m_sampler);

			ML_GL.texParameter(
				m_sampler,
				GL::TexMinFilter,
				(((m_smooth)
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest
					)));

			ML_GL.texParameter(
				m_sampler,
				GL::TexMagFilter,
				(((m_smooth)
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest
					)));
			
			this->unbind();

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setRepeated(bool value)
	{
		if (*this)
		{
			if (((m_repeated) = value) && !(ML_GL.edgeClampAvailable()))
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning(
						"OpenGL extension texture_edge_clamp unavailable\n"
						"Artifacts may occur along texture edges"
					);
					warned = true;
				}
			}

			this->bind();
			
			ML_GL.texParameter(
				m_sampler,
				GL::TexWrapS,
				((m_repeated)
					? GL::Repeat
					: ((ML_GL.edgeClampAvailable())
						? GL::ClampToEdge
						: GL::Clamp
						)));

			ML_GL.texParameter(
				m_sampler,
				GL::TexWrapT,
				((m_repeated)
					? GL::Repeat
					: ((ML_GL.edgeClampAvailable())
						? GL::ClampToEdge
						: GL::Clamp
						)));
			
			this->unbind();

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if (*this)
		{
			m_smooth = value;

			this->bind();

			ML_GL.texParameter(
				m_sampler,
				GL::TexMinFilter,
				((m_smooth)
					? GL::Linear
					: GL::Nearest
					));

			ML_GL.texParameter(
				m_sampler,
				GL::TexMagFilter,
				((m_smooth)
					? GL::Linear
					: GL::Nearest
					));

			this->unbind();

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setSampler(GL::Sampler value)
	{
		if (*this)
		{
			this->bind();
			this->unbind();
			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setLevel(int32_t value)
	{
		if (*this)
		{
			this->bind();
			//ML_GL.texParameter(m_sampler, GL::BaseLevel, value);
			//ML_GL.texParameter(m_sampler, GL::MaxLevel, value);
			this->unbind();
			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setInternalFormat(GL::Format value)
	{
		if (*this)
		{
			this->bind();
			this->unbind();
			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setColorFormat(GL::Format value)
	{
		if (*this)
		{
			this->bind();
			this->unbind();
			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setType(GL::Type value)
	{
		if (*this)
		{
			this->bind();
			this->unbind();
			ML_GL.flush();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::swap(Texture & other)
	{
		std::swap(get_reference(),	other.get_reference());
		std::swap(m_sampler,		other.m_sampler);
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const Image Texture::copyToImage() const
	{
		Image image;
		if (*this)
		{
			Image::Pixels pixels(width() * height() * 4);

			if ((m_size == m_realSize))
			{
				this->bind();
				ML_GL.getTexImage(
					m_sampler,
					m_level,
					m_internalFormat,
					m_type,
					&pixels[0]
				);
				this->unbind();
			}

			return image.update(width(), height(), pixels);
		}
		return image;
	}

	const Texture & Texture::bind() const
	{
		bind(this);
		return (*this);
	}

	const Texture & Texture::unbind() const
	{
		bind(nullptr);
		return (*this);
	}

	void Texture::bind(const Texture * value)
	{
		return ((value)
			? ML_GL.bindTexture(value->sampler(), (*value))
			: ML_GL.bindTexture(GL::Texture2D, NULL)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}