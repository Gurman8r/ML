#ifndef _ML_SCENE_HPP_
#define _ML_SCENE_HPP_

#include <ML/Graphics/Drawable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	struct ML_GRAPHICS_API Scene final
		: public Newable
		, public Drawable
		, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Scene();
		~Scene();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool saveToFile(const String & filename) const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SCENE_HPP_