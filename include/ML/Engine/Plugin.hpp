#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/EngineEvents.hpp>
#include <ML/Core/EventListener.hpp>
#include <ML/Core/MemoryTracker.hpp>

#define ML_PLUGIN_API ML_API_EXPORT

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct EventSystem;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Plugin : public Trackable, public NonCopyable, public EventListener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Plugin() {}
		
		virtual ~Plugin() {}

		virtual void onEvent(Event const & value) override = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PLUGIN_HPP_