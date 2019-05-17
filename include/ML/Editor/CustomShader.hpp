#ifndef _ML_CUSTOM_SHADER_HPP_
#define _ML_CUSTOM_SHADER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Graphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API CustomShader final
		: public ITrackable
	{
	public:
		CustomShader();
		~CustomShader();

	private:
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CUSTOM_SHADER_HPP_