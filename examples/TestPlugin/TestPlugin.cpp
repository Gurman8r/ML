/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Test(void * data)
{
	if (std::strcmp((ml::CString)data, "TEST"))
	{
		return (void *)("Hello from Test Plugin!");
	}
	else
	{
		return (void *)("Test Plugin Handled an Error!");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */