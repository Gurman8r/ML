#ifndef _ML_ENGINE_HPP_
#define _ML_ENGINE_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IEventListener.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class EventSystem;
	class Preferences;
	class RenderWindow;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Engine final
		: public ITrackable
		, public INonCopyable
		, public IEventListener
	{
	public:
		explicit Engine(
			EventSystem		& eventSystem,
			Preferences		& prefs,
			RenderWindow	& window
		);

		~Engine();

	public:
		void onEvent(const IEvent * value) override;

	private:
		EventSystem		& m_eventSystem;
		Preferences		& m_prefs;
		RenderWindow	& m_window;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ENGINE_HPP_