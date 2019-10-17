#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/OpenGL.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture::Texture() 
		: Texture { GL::Texture2D }
	{
	}

	Texture::Texture(GL::Sampler sampler) 
		: Texture { sampler, true, false }
	{
	}

	Texture::Texture(bool smooth, bool repeated) 
		: Texture { GL::Texture2D, smooth, repeated }
	{
	}

	Texture::Texture(GL::Format format, bool smooth, bool repeated) 
		: Texture { GL::Texture2D, format, smooth, repeated }
	{
	}

	Texture::Texture(GL::Sampler sampler, bool smooth, bool repeated) 
		: Texture { sampler, GL::RGBA, smooth, repeated }
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated)
		: Texture { sampler, format, smooth, repeated, false }
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped) 
		: Texture { sampler, format, format, smooth, repeated, mipmapped }
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type)
		: Texture { sampler, format, format, smooth, repeated, mipmapped, level, type }
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated) 
		: Texture { sampler, iFormat, cFormat, smooth, repeated, false }
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated, bool mipmapped) 
		: Texture { sampler, iFormat, cFormat, smooth, repeated, mipmapped, 0, GL::UnsignedByte }
	{
	}

	Texture::Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type pixType)
		: Handle		{ NULL }
		, m_size		{ 0, 0 }
		, m_realSize	{ 0, 0 }
		, m_sampler		{ sampler }
		, m_iFormat		{ iFormat }
		, m_cFormat		{ cFormat }
		, m_smooth		{ smooth }
		, m_repeated	{ repeated }
		, m_mipmapped	{ mipmapped }
		, m_level		{ level }
		, m_pixType		{ pixType }
	{
	}

	Texture::Texture(const Texture & copy) : Texture {
		copy.m_sampler,
		copy.m_iFormat,
		copy.m_cFormat,
		copy.m_smooth,
		copy.m_repeated,
		copy.m_mipmapped,
		copy.m_level,
		copy.m_pixType
	}
	{
		create(copy);
	}

	Texture::Texture(const Image & image)
		: Texture {}
	{
		loadFromImage(image);
	}

	Texture::~Texture() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Texture::dispose()
	{
		unbind();
		if (*this)
		{
			ML_GL.deleteTexture(*this);

			set_handle(NULL);
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
		if (!value.channels()) return false;
		m_iFormat = m_cFormat = value.getFormat();
		return create(value.size()) && update(value);
	}

	bool Texture::loadFromFaces(const Array<const Image *, 6> & faces)
	{
		// Validate Sampler
		if (m_sampler != GL::TextureCubeMap)
		{
			return Debug::logError("Load from faces only available for {0}s.",
				GL::TextureCubeMap
			);
		}

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
			else if (faces[i]->channels() != faces[0]->channels())
			{
				return Debug::logError("Face channel mismatch: {0} -> {1}", 
					faces[0]->channels(),
					faces[i]->channels()
				);
			}
		}

		m_iFormat = m_cFormat = faces[0]->getFormat();

		// Create Texture
		if (dispose() && set_handle(ML_GL.genTexture()))
		{
			bind();

			for (size_t i = 0; i < faces.size(); i++)
			{
				ML_GL.texImage2D(
					GL::CubeMap_Positive_X + (uint32_t)i,
					m_level,
					m_iFormat,
					faces[i]->size()[0],
					faces[i]->size()[1],
					0,
					m_cFormat,
					m_pixType,
					faces[i]->data()
				);
			}
			
			unbind();
			
			ML_GL.flush();
			
			setRepeated(m_repeated);
			
			setSmooth(m_smooth);
			
			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::create(const Texture & other)
	{
		return ((other)
			? (create(other.size())
				? update(other)
				: Debug::logError("Failed to copy texture, failed to create new texture")) 
			: false
		);
	}

	bool Texture::create(const vec2u & size)
	{
		return create(nullptr, size);
	}

	bool Texture::create(const Image & image, const vec2u & size)
	{
		return create(image.data(), size);
	}

	bool Texture::create(const Image & image, uint32_t w, uint32_t h)
	{
		return create(image.data(), w, h);
	}

	bool Texture::create(const byte_t * pixels, const vec2u & size)
	{
		return create(pixels, size[0], size[1]);
	}

	bool Texture::create(const byte_t * pixels, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if (dispose() && set_handle(ML_GL.genTexture()))
			{
				m_size = { w, h };
				m_realSize =
				{
					ML_GL.getValidTextureSize(m_size[0]),
					ML_GL.getValidTextureSize(m_size[1])
				};

				static const uint32_t max_size { ML_GL.getMaxTextureSize() };
				if ((m_realSize[0] > max_size) || (m_realSize[1] > max_size))
				{
					return Debug::logError(
						"Failed creating texture, size is too large {0} max is {1}",
						m_realSize, 
						vec2u { max_size , max_size }
					);
				}

				bind();

				ML_GL.texImage2D(
					m_sampler,		// 
					m_level,		// 
					m_iFormat,		// 
					m_size[0],		//  
					m_size[1],		// 
					0,				// border: "This value must be 0" -khronos.org
					m_cFormat,		// 
					m_pixType,		// 
					pixels			// 
				);
				
				unbind();

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
			return Debug::logError("Failed creating texture, invalid size: {0} x {1}", w, h);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
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
		return update(image.data(), image.bounds());
	}

	bool Texture::update(const Image & image, const UintRect & area)
	{
		return update(image.data(), area.position(), area.size());
	}

	bool Texture::update(const Image & image, const vec2u & position, const vec2u & size)
	{
		return update(image.data(), position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(const Image & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		return update(image.data(), x, y, w, h);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::update(const byte_t * pixels)
	{
		return update(pixels, UintRect { width(), height() });
	}

	bool Texture::update(const byte_t * pixels, const UintRect & area)
	{
		return update(pixels, area.position(), area.size());
	}

	bool Texture::update(const byte_t * pixels, const vec2u & position, const vec2u & size)
	{
		return update(pixels, position[0], position[1], size[0], size[1]);
	}

	bool Texture::update(const byte_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		if (w && h)
		{
			if ((*this) && (pixels))
			{
				bind();

				ML_GL.texSubImage2D(
					m_sampler,	// 
					m_level,	// 
					x,			//  
					y,			// 
					w,			//  
					h,			// 
					m_iFormat,	// 
					m_pixType,	// 
					pixels		// 
				);

				unbind();

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
				static bool once { true };
				if (once && !(once = false))
				{
					Debug::logWarning("Texture Mipmap Framebuffers Unavailable");
				}
				m_mipmapped = false;
				return setSmooth(m_smooth);
			}

			bind();

			ML_GL.generateMipmap(m_sampler);

			ML_GL.texParameter(
				m_sampler,
				GL::TexMinFilter,
				m_smooth ? GL::LinearMipmapLinear : GL::NearestMipmapNearest
			);

			ML_GL.texParameter(
				m_sampler, 
				GL::TexMagFilter, 
				m_smooth ? GL::LinearMipmapLinear : GL::NearestMipmapNearest
			);
			
			unbind();

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setRepeated(bool value)
	{
		if (*this)
		{
			if ((m_repeated = value) && !ML_GL.edgeClampAvailable())
			{
				static bool once { true };
				if (once && !(once = false))
				{
					Debug::logWarning(
						"OpenGL extension texture_edge_clamp unavailable.\n"
						"Artifacts may occur along texture edges."
					);
				}
			}

			bind();
			
			ML_GL.texParameter(
				m_sampler,
				GL::TexWrapS,
				(m_repeated
					? GL::Repeat
					: (ML_GL.edgeClampAvailable()) ? GL::ClampToEdge : GL::Clamp)
			);

			ML_GL.texParameter(
				m_sampler,
				GL::TexWrapT,
				(m_repeated
					? GL::Repeat
					: (ML_GL.edgeClampAvailable()) ? GL::ClampToEdge : GL::Clamp)
			);
			
			unbind();

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if (*this)
		{
			m_smooth = value;

			bind();

			ML_GL.texParameter(
				m_sampler, 
				GL::TexMinFilter,
				(m_smooth ? GL::Linear : GL::Nearest)
			);

			ML_GL.texParameter(
				m_sampler, 
				GL::TexMagFilter, 
				(m_smooth ? GL::Linear : GL::Nearest)
			);

			unbind();

			ML_GL.flush();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::swap(Texture & other)
	{
		std::swap(m_handle,		other.m_handle);
		std::swap(m_sampler,	other.m_sampler);
		std::swap(m_level,		other.m_level);
		std::swap(m_size,		other.m_size);
		std::swap(m_realSize,	other.m_realSize);
		std::swap(m_iFormat,	other.m_iFormat);
		std::swap(m_cFormat,	other.m_cFormat);
		std::swap(m_pixType,	other.m_pixType);
		std::swap(m_smooth,		other.m_smooth);
		std::swap(m_repeated,	other.m_repeated);
		std::swap(m_mipmapped,	other.m_mipmapped);
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
		Image image { size(), getChannels() };
		if (*this)
		{
			bind();
			
			ML_GL.getTexImage(GL::Texture2D, m_level, m_iFormat, m_pixType, &image[0]);
			
			unbind();
		}
		return image;
	}

	uint32_t Texture::getChannels() const
	{
		switch (m_iFormat)
		{
		case ml::GL::Red: return 1;
		case ml::GL::RGB: return 3;
		case ml::GL::RGBA: return 4;
		default: return 0;
		}
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