#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <ML/Graphics/Image.hpp>
#include <ML/Core/I_Handle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Texture final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_Handle<uint32_t>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture();
		explicit Texture(GL::Sampler sampler);
		explicit Texture(bool smooth, bool repeated);
		explicit Texture(GL::Format format, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped);
		explicit Texture(GL::Sampler sampler, GL::Format format, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type);
		explicit Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated);
		explicit Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated, bool mipmapped);
		explicit Texture(GL::Sampler sampler, GL::Format iFormat, GL::Format cFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, GL::Type type);
		explicit Texture(const Texture & copy);
		~Texture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromImage(const Image & value);
		bool loadFromFaces(const Array<const Image *, 6> & faces);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(const Texture & copy);
		bool create(const vec2u & size);
		bool create(const Image & image, const vec2u & size);
		bool create(const Image & image, uint32_t w, uint32_t h);
		bool create(const byte_t * pixels, const vec2u & size);
		bool create(const byte_t * pixels, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(const Texture & other);
		bool update(const Texture & other, const UintRect & area);
		bool update(const Texture & other, const vec2u & position, const vec2u & size);
		bool update(const Texture & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		bool update(const Image & image);
		bool update(const Image & image, const UintRect & area);
		bool update(const Image & image, const vec2u & position, const vec2u & size);
		bool update(const Image & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		
		bool update(const byte_t * pixels);
		bool update(const byte_t * pixels, const UintRect & area);
		bool update(const byte_t * pixels, const vec2u & position, const vec2u & size);
		bool update(const byte_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & setMipmapped(bool value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & swap(Texture & value);
		Texture & operator=(const Texture & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Image copyToImage() const;

		uint32_t getChannels() const;

		const Texture & bind() const;

		const Texture & unbind() const;

		static void	bind(const Texture * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto sampler()		const -> GL::Sampler	{ return m_sampler; }
		inline auto level()			const -> int32_t		{ return m_level; }
		inline auto internal_fmt()	const -> GL::Format		{ return m_iFormat; }
		inline auto color_fmt()		const -> GL::Format		{ return m_cFormat; }
		inline auto pixel_type()	const -> GL::Type		{ return m_pixType; }
		inline auto size()			const -> const vec2u &	{ return m_size; }
		inline auto realSize()		const -> const vec2u &	{ return m_realSize; }
		inline auto smooth()		const -> bool			{ return m_smooth; }
		inline auto repeated()		const -> bool			{ return m_repeated; }
		inline auto mipmapped()		const -> bool			{ return m_mipmapped; }
		inline auto width()			const -> uint32_t		{ return m_size[0]; }
		inline auto height()		const -> uint32_t		{ return m_size[1]; }
		inline auto real_width()	const -> uint32_t		{ return m_realSize[0]; }
		inline auto real_height()	const -> uint32_t		{ return m_realSize[1]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		GL::Sampler	m_sampler;		// Sampler Type
		int32_t		m_level;		// Texture Level
		GL::Format	m_iFormat;		// Internal Format
		GL::Format	m_cFormat;		// Color Format
		GL::Type	m_pixType;		// Storage Type
		vec2u		m_size;			// Texture Size
		vec2u		m_realSize;		// Real Texture Size
		bool		m_smooth;		// Is Smooth
		bool		m_repeated;		// Is Repeated
		bool		m_mipmapped;	// Is Mipmapped

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_