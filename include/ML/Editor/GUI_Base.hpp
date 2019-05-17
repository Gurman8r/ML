#ifndef _ML_GUI_BASE_HPP_
#define _ML_GUI_BASE_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	class ML_EDITOR_API GUI_Base
		: public ITrackable
	{
	public:
		GUI_Base();
		virtual ~GUI_Base();

	public:
		inline bool	good() const 
		{ 
			return (m_good);
		}

	protected:
		inline bool goodCheck(const bool value)
		{ 
			return (m_good = value); 
		}

	private:
		bool m_good;
	};
}

#endif // !_ML_GUI_BASE_HPP_