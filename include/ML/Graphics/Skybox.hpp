#ifndef _ML_SKYBOX_HPP_
#define _ML_SKYBOX_HPP_

#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/IDrawable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	class ML_GRAPHICS_API Skybox final
		: public IObject
		, public IDisposable
		, public IReadable
	{
	public:
		Skybox();
		~Skybox();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SKYBOX_HPP_