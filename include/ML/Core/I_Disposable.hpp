#ifndef _ML_I_DISPOSABLE_HPP_
#define _ML_I_DISPOSABLE_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class I_Disposable
	{
	public:
		virtual bool dispose() = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_DISPOSABLE_HPP_