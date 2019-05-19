#ifndef _ML_NETWORK_HUD_HPP_
#define _ML_NETWORK_HUD_HPP_

#include <ML/Editor/BaseWidget.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API NetworkHUD final
		: public BaseWidget
	{
		friend class Editor;

	private:
		NetworkHUD(EventSystem & eventSystem);
		~NetworkHUD();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(const GuiEvent * ev, bool * p_open) override;

	public:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_HUD_HPP_