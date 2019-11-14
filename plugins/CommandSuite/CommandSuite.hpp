#ifndef _COMMAND_SUITE_HPP_
#define _COMMAND_SUITE_HPP_

#include <ML/Engine/Plugin.hpp>
#include <ML/Engine/CommandImpl.hpp>

extern "C" ML_PLUGIN_API ml::ptr_t<ml::Plugin> ML_Plugin_Main();

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PLUGIN_API CommandSuite final : public Plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		explicit CommandSuite();

		~CommandSuite() {}

		void onEvent(const Event & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_COMMAND_SUITE_HPP_