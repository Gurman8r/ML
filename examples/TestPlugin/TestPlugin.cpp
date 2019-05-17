/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Test(void * data)
{
	if ((ml::String)((ml::CString)data) == "TEST")
	{
		return (void *)("Hello from Test Plugin!");
	}
	else
	{
		return NULL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */