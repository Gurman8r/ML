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
		/* * * * * * * * * * * * * * * * * * * * */
		NetworkHUD(Editor & editor, bool open);
		~NetworkHUD();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const DrawGuiEvent & ev) override;

	public:
		/* * * * * * * * * * * * * * * * * * * * */

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_HUD_HPP_