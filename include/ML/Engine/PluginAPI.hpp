#ifndef _ML_PLUGIN_API_HPP_
#define _ML_PLUGIN_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Engine/Export.hpp>
#include <ML/Core/Preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	class Application;
	class EventSystem;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PLUGIN_API	ML_API_EXPORT
#define ML_Plugin_Test	ML_Plugin_Test
#define ML_Plugin_Main	ML_Plugin_Main

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Test(void *);

	ML_PLUGIN_API ml::Application * ML_Plugin_Main(ml::EventSystem &);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PLUGIN_API_HPP_