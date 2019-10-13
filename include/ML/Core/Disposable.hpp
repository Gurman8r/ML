#ifndef _ML_DISPOSABLE_HPP_
#define _ML_DISPOSABLE_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Disposable { virtual bool dispose() = 0; };

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DISPOSABLE_HPP_