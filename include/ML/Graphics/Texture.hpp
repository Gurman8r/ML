#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <ML/Core/Rect.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Graphics/Image.hpp>
#include <ML/Core/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Texture final
		: public IObject
		, public IDisposable
		, public IReadable
		, public IHandle<uint32_t>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Texture();
		Texture(GL::Target target);
		Texture(bool smooth, bool repeated);
		Texture(GL::Format format, bool smooth, bool repeated);
		Texture(GL::Target target, bool smooth, bool repeated);
		Texture(GL::Target target, GL::Format format, bool smooth, bool repeated);
		Texture(GL::Target target, GL::Format format, bool smooth, bool repeated, bool mipmapped);
		Texture(GL::Target target, GL::Format internalFormat, GL::Format colorFormat, bool smooth, bool repeated);
		Texture(GL::Target target, GL::Format internalFormat, GL::Format colorFormat, bool smooth, bool repeated, bool mipmapped);
		Texture(GL::Target target, GL::Format internalFormat, GL::Format colorFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type);
		Texture(const Texture & copy);
		~Texture();

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromImage(const Image & value);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool create(const Texture & copy);
		bool create(const vec2u & size);
		bool create(const Image & image, const vec2u & size);
		bool create(const Image & image, uint32_t w, uint32_t h);
		bool create(const uint8_t * pixels, const vec2u & size);
		bool create(const uint8_t * pixels, uint32_t w, uint32_t h);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool update(const Texture & other);
		bool update(const Texture & other, const UintRect & area);
		bool update(const Texture & other, const vec2u & position, const vec2u & size);
		bool update(const Texture & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		bool update(const Image & image);
		bool update(const Image & image, const UintRect & area);
		bool update(const Image & image, const vec2u & position, const vec2u & size);
		bool update(const Image & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		
		bool update(const uint8_t * pixels);
		bool update(const uint8_t * pixels, const UintRect & area);
		bool update(const uint8_t * pixels, const vec2u & position, const vec2u & size);
		bool update(const uint8_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Texture & setMipmapped(bool value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Texture & swap(Texture & value);
		Texture & operator=(const Texture & value);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		const Image copyToImage() const;
		static void	bind(const Texture * value);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline const GL::Target	target()		const { return m_target;		}
		inline const int32_t	level()			const { return m_level;			}
		inline const GL::Format internalFormat()const { return m_internalFormat;}
		inline const GL::Format colorFormat()	const { return m_colorFormat;	}
		inline const GL::Type	type()			const { return m_type;			}
		inline const vec2u &	size()			const { return m_size;			}
		inline const vec2u &	realSize()		const { return m_realSize;		}
		inline const bool		smooth()		const { return m_smooth;		}
		inline const bool		repeated()		const { return m_repeated;		}
		inline const bool		mipmapped()		const { return m_mipmapped;		}
		inline const uint32_t	width()			const { return m_size[0];		}
		inline const uint32_t	height()		const { return m_size[1];		}
		inline const uint32_t	realWidth()		const { return m_realSize[0];	}
		inline const uint32_t	realHeight()	const { return m_realSize[1];	}

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		GL::Target	m_target;			// 
		int32_t		m_level;			// 
		GL::Format	m_internalFormat;	// 
		GL::Format	m_colorFormat;		// 
		GL::Type	m_type;				// 
		vec2u		m_size;				// 
		vec2u		m_realSize;			// 
		bool		m_smooth;			// 
		bool		m_repeated;			// 
		bool		m_mipmapped;		// 
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_