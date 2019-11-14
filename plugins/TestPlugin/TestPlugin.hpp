#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

#include <ML/Engine/Plugin.hpp>

extern "C" ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main();

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API TestPlugin final : public Plugin
	{
		TestPlugin();
		
		~TestPlugin() {}

		void onEvent(const Event & value) override;
	};
	
	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_TEST_PLUGIN_HPP_